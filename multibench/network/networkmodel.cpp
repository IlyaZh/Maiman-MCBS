#include "networkmodel.h"
#
#include <QDebug>
#include "model/device/devicepollrequest.h"
#include <QTcpSocket>
#include "datasource.h"
#include <QDateTime>
#include "protocols/modbusprotocol.h"
#include "SoftProtocol.h"
#include "device/devicemodel.h"
#include "model/devicefactory.h"
#include "mainfacade.h"
#include "appsettings.h"

const quint16 NetworkModel::TIMEOUT_MS = 50*10;
const quint16 NetworkModel::IDENTIFY_REG_ID_DEFAULT = 0x0001; // debug замени

NetworkModel::NetworkModel(DeviceFactory &deviceModelFactory, SoftProtocol& protocol, MainFacade& facade, QObject *parent) :
    QObject(parent),
    m_deviceModelFactory(deviceModelFactory),
    m_facade(facade),
    m_protocol(protocol)
{
    m_bIsStart = false;
    m_deviceModelFactory.start();
    connect(&m_deviceModelFactory,&DeviceFactory::parsingIsFinished, this, &NetworkModel::getBaudrate);
    //    connect(&m_delayTimer, &QTimer::timeout, this, &NetworkModel::delayTimeout);
    connect(&m_timeoutTimer, &QTimer::timeout, this, &NetworkModel::sendTimeout);

    //    m_delayTimer.setSingleShot(true);
    m_timeoutTimer.setSingleShot(true);
}

NetworkModel::~NetworkModel() {
    clear();
}
// controller to model interface overrides

void NetworkModel::getBaudrate(){
    m_facade.setBaudRates(m_deviceModelFactory.getBaudrate());
}

void NetworkModel::setDelay(int delay) {
    m_delayMs = delay;
}

void NetworkModel::setTimeout(int timeout) {
    m_timeoutMs = timeout;
}


void NetworkModel::start(DataSource& networkDevice)
{
    //m_facade.setBaudRates(m_deviceModelFactory.getBaudrate());
    qDebug()<<m_deviceModelFactory.getBaudrate();
    if(!m_port.isNull()) {
        m_port->disconnect();
        //        m_port->deleteLater();
    }
    m_port.reset(&networkDevice);
    connect(m_port.get(), &DataSource::bytesWritten, this, &NetworkModel::bytesWritten);
    connect(m_port.get(), &DataSource::readyRead, this, &NetworkModel::readyRead);
    connect(m_port.get(), &DataSource::errorOccured, this, &NetworkModel::errorOccured);
    connect(m_port.get(), &DataSource::deviceOpen, this, [=](bool state){
        qDebug() << "NetworkModel protocol state" << state;
    });
    m_portIsBusy = false;

    m_bIsStart = true;

    rescanNetwork();
}

bool NetworkModel::isStart() {
    return m_bIsStart;
}

void NetworkModel::stop()
{
    clear();
    m_bIsStart = false;
    m_portIsBusy = false;
}

void NetworkModel::rescanNetwork()
{
    clear();
    QSet<quint8> addresses(AppSettings::getDeviceAddresses());//TODO: dont work
    if (!AppSettings::getKeepAddresses()){
        addresses.clear();
        clearNetwork();
    }
    if (addresses.isEmpty()){
        for(quint8 iAddr = 1; iAddr <= SoftProtocol::MaxAddress; ++iAddr) {
            addresses.insert(iAddr);
        }
    }
    for(const auto item:addresses){
        m_queue.enqueue(m_protocol.getDataValue(item, NetworkModel::IDENTIFY_REG_ID_DEFAULT));
    }

    tryToSend();
}

void NetworkModel::clearNetwork(){
    QSet<quint8> addresses;
    AppSettings::setDeviceAddresses(addresses);
}

// private methods
void NetworkModel::clear() {
    m_delayTimer.stop();
    m_timeoutTimer.stop();
    m_priorityQueue.clear();
    m_queue.clear();
    for(auto& item : m_devices) {
        item->disconnect();
        item->deleteLater();
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

    m_facade.createWidgetFor(newDevice.get());
}

void NetworkModel::tryToSend() {
    if(!m_portIsBusy) {
        m_portIsBusy = true;
        if(!m_port.isNull()) {
            if (!m_priorityQueue.isEmpty()) {
                m_lastTxPackage = m_priorityQueue.dequeue();
            } else if (!m_queue.isEmpty()) {
                m_lastTxPackage = m_queue.dequeue();
            } else {
                // poll devices state
                for(const auto& dev : qAsConst(m_devices)) {
                    const DevicePollRequest request = dev->nextPollRequest();
                    if(request.code != 0) {
                        m_queue.enqueue(m_protocol.getDataValue(request.addr, request.code, request.count));
                    }
                }
                if(!m_queue.isEmpty())
                    m_lastTxPackage = m_queue.dequeue();
                //            tryToSend();
                //                delayTimeout();
            }
            if(!m_lastTxPackage.isEmpty()) {
                m_port->write(m_lastTxPackage);
                m_waitForBytes = m_protocol.waitForBytes(m_lastTxPackage);
            }
            //        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "Write! " << m_lastTxPackage.toHex(' ');
        }
    }

}

// public slots
// modbus interface overrides

void NetworkModel::dataOutcome(quint8 addr, quint16 reg, quint16 value)
{
    m_priorityQueue.enqueue(m_protocol.setDataValue(addr, reg, value));
    tryToSend();
}

void NetworkModel::temperatureUnitsChanged(Const::TemperatureUnitId id) {
    qDebug() << "NetworkModel::temperatureUnitsChanged" << static_cast<int>(id);

    for(auto& device : m_devices) {
        device->changeTemperatureUnit(id);
    }
}

// private slots
void NetworkModel::readyRead() {
    m_rxPacket.append(m_port->readAll());
    if(m_rxPacket.size() >= m_waitForBytes) {
        //    qDebug() << "RX" << rxPacket.toHex(' ');
        m_timeoutTimer.stop();
        SoftProtocol::DataVector result = m_protocol.execute(m_rxPacket, m_lastTxPackage);

        m_lastTxPackage.clear();
        if(m_protocol.isError()) {
            quint8 addr = static_cast<quint8>(m_rxPacket.at(0));
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
        m_rxPacket.clear();
        m_portIsBusy = false;
        tryToSend();
    }
}

void NetworkModel::bytesWritten(qint64 bytes) {
    m_bytesWritten += bytes;
    if(m_bytesWritten >= m_lastTxPackage.size()) {
        //        qDebug() << "Written bytes";
        m_bytesWritten = 0;
        if(m_protocol.needWaitForAnswer(m_lastTxPackage)) {
            m_timeoutTimer.start(m_timeoutMs);
            //            qDebug() << "Start interval timeout";
        } else {
            tryToSend();
        }
    }
}

void NetworkModel::errorOccured(const QString& msg) {
    qWarning() << "NetworkModel error" << msg;
}

void NetworkModel::sendTimeout() {
    m_timeoutTimer.stop();
    m_portIsBusy = false;

    if(!m_lastTxPackage.isEmpty()) {
        quint8 addr = static_cast<quint8>(m_lastTxPackage.at(0));
        auto device = m_devices.value(addr);
        if(device) {
            device->unlink();
        }
    }
    m_lastTxPackage.clear();

    //    qDebug() << "Timeout";
    tryToSend();
}

//void NetworkModel::delayTimeout() {
////    m_delayTimer.stop();
//    if(!m_port.isNull()) {
//        if (!m_priorityQueue.isEmpty()) {
//            m_lastTxPackage = m_priorityQueue.dequeue();
//        } else if (!m_queue.isEmpty()) {
//            m_lastTxPackage = m_queue.dequeue();
//        } else {
//            // poll devices state
//            for(const auto& dev : qAsConst(m_devices)) {
//                const DevicePollRequest request = dev->nextPollRequest();
//                if(request.code != 0) {
//                    m_queue.enqueue(m_protocol.getDataValue(request.addr, request.code, request.count));
//                }
//            }
//            //            tryToSend();
//            delayTimeout();
//            return;
//        }

//        m_port->write(m_lastTxPackage);
////        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "Write! " << m_lastTxPackage.toHex(' ');
//    }
//}
// =====================

