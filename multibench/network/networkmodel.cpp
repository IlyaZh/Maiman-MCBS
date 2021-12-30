#include "networkmodel.h"

#include <QDebug>
#include "model/device/devicepollrequest.h"
#include <QTcpSocket>
#include "datasource.h"
#include <QDateTime>
#include "protocols/modbusprotocol.h"
#include "SoftProtocol.h"
#include "device/devicemodel.h"
#include "model/devicefactory.h"
#include "appsettings.h"
#include "DataThread.h"
#include "network/IDataSource.h"

const quint16 NetworkModel::TIMEOUT_MS = 50*10;
const quint16 NetworkModel::IDENTIFY_REG_ID_DEFAULT = 0x0001; // debug замени

NetworkModel::NetworkModel(DeviceFactory &deviceModelFactory, SoftProtocol& protocol, QObject *parent) :
    QObject(parent),
    m_deviceModelFactory(deviceModelFactory),
    m_protocol(protocol)
{
    m_isStart = false;
    m_deviceModelFactory.start();
    connect(&m_deviceModelFactory,&DeviceFactory::parsingIsFinished, this, &NetworkModel::getBaudrate);
}

NetworkModel::~NetworkModel() {
    clear();
}
// controller to model interface overrides

void NetworkModel::getBaudrate(){
    emit signal_setBaudrateToWindow(m_deviceModelFactory.getBaudrate());
}

void NetworkModel::setTimeout(int timeout) {
    m_timeoutMs = timeout;
    if(m_worker)
        m_worker->setTimeout(m_timeoutMs);
}


void NetworkModel::start(QScopedPointer<IDataSource>& source)
{
    auto thread = new QThread;
    m_worker = new DataThread;
    m_worker->setTimeout(m_timeoutMs);
    m_worker->configure(source);
    m_worker->moveToThread(thread);

    connect(m_worker, &DataThread::connected, this, [this]() {
        emit signal_connected(true);
        m_isStart = true;
        rescanNetwork();
    });
    connect(m_worker, &DataThread::readyToWrite, this, &NetworkModel::pollRequest);
    connect(m_worker, &DataThread::timeout, this, &NetworkModel::timeout);
    connect(m_worker, &DataThread::errorOccured, this, &NetworkModel::signal_errorOccured);
    connect(m_worker, &DataThread::readyRead, this, &NetworkModel::readyRead);
    connect(m_worker, &DataThread::finished, this, [this, &thread](){
        emit signal_connected(false);
        m_isStart = false;
        thread->quit();
        m_worker->deleteLater();
    });
    connect(thread, &QThread::started, m_worker, &DataThread::process);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
//        m_worker->start();
    thread->start();
}

bool NetworkModel::isStart() {
    return m_isStart;
}

void NetworkModel::stop()
{
    if(m_worker) {
        m_worker->stop();
    }
    clear();
    m_isStart = false;
}

QMap<quint16, QSharedPointer<DevCommand>> NetworkModel::getCommands(quint8 addr){
    return m_devices.value(addr)->commands();
}

void NetworkModel::rescanNetwork()
{
    clear();
    QSet<quint8> addresses(AppSettings::getDeviceAddresses());
    if (!AppSettings::getKeepAddresses()){
        addresses.clear();
        clearNetwork();
    }
    if (addresses.isEmpty()){
        for(quint8 iAddr = 1; iAddr <= SoftProtocol::MaxAddress; ++iAddr) {
            addresses.insert(iAddr);
        }
    }

    if(m_worker) {
        for(const auto item : addresses){
            auto package = m_protocol.getDataValue(item, NetworkModel::IDENTIFY_REG_ID_DEFAULT);
            qint64 waitForBytes = m_protocol.waitForBytes(package);
            m_worker->writeAndWaitBytes(package, waitForBytes, true);
        }
    }
}

void NetworkModel::clearNetwork(){
    QSet<quint8> addresses;
    AppSettings::setDeviceAddresses(addresses);
}

// private methods
void NetworkModel::clear() {
    for(auto& item : m_devices) {
        item->disconnect();
    }
    m_devices.clear();
}

void NetworkModel::initDevice(quint8 addr, quint16 id)
{
    qDebug() << "Init device " << addr << " with id " << id;
    QSharedPointer<Device> newDevice =  m_deviceModelFactory.createDevice(addr, id);
    if(newDevice.isNull()) {
        qDebug() << "Device with id " << id << "hasn't found in config";
        return;
    }

    if(m_devices.contains(addr)) {
        auto &oldDev = m_devices[addr];
        oldDev->disconnect();
        oldDev.reset();
        m_devices.remove(addr);
    }

    m_devices.insert(addr, newDevice);

    QSet<quint8> addresses(AppSettings::getDeviceAddresses());
    addresses.insert(addr);
    AppSettings::setDeviceAddresses(addresses);

    connect(newDevice.get(), SIGNAL(dataToModel(quint8,quint16,quint16)), this, SLOT(dataOutcome(quint8,quint16,quint16)));

    emit signal_createWidgetFor(newDevice.get());
}

// public slots
// modbus interface overrides

void NetworkModel::dataOutcome(quint8 addr, quint16 reg, quint16 value)
{
    if(m_worker) {
        auto package = m_protocol.setDataValue(addr, reg, value);
        qint64 waitForBytes = m_protocol.waitForBytes(package);
        m_worker->writeAndWaitBytes(package, waitForBytes, true);
    }
}

void NetworkModel::temperatureUnitsChanged(Const::TemperatureUnitId id) {
    for(auto& device : m_devices) {
        device->changeTemperatureUnit(id);
    }
}

// private slots
void NetworkModel::timeout() {
    if(m_worker) {
        auto package = m_worker->lastPackage();
        if(!package.isEmpty()) {
            auto device = m_devices.value(static_cast<quint8>(package.at(0)));
            if(device) {
                qDebug() << "TIMEOUT UNLINK DEV" << device->addr();
                device->unlink();
            }
        }
    }
}

void NetworkModel::pollRequest() {
    if(m_worker) {
        // poll devices state
        qDebug() << "poll request";
        for(const auto& dev : qAsConst(m_devices)) {
            const DevicePollRequest request = dev->nextPollRequest();
            qDebug() << request.code << request.addr << request.count;
            if(request.code != 0) {
                auto package = m_protocol.getDataValue(request.addr, request.code, request.count);
                qint64 waitForBytes = m_protocol.waitForBytes(package);
                m_worker->writeAndWaitBytes(package, waitForBytes, true);
            }
        }
    }
}

void NetworkModel::readyRead(const QByteArray& rxPackage) {
    auto result = m_protocol.execute(rxPackage, m_worker->lastPackage());
    qDebug() << "NetworkModel::readyRead" << m_protocol.isError() << rxPackage.toHex(' ');
    if(m_protocol.isError()) {
        qDebug() << "PROTOCOL ERROR" << m_protocol.errorString();
        quint8 addr = static_cast<quint8>(rxPackage.at(0));
        if(m_devices.contains(addr)) {
            m_devices[addr]->unlink();
        }
    } else {
        for(const auto& item : qAsConst(result)) {
            if(item.reg == NetworkModel::IDENTIFY_REG_ID_DEFAULT) {
                initDevice(item.addr, item.value);
            } else {
                if(m_devices.contains(item.addr)) {
                    m_devices[item.addr]->dataIncome(item.reg, item.value);
                }
            }
        }
    }
}



