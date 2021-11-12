#include "networkmodel.h"
#include <QDebug>
#include "model/device/devicepollrequest.h"
#include <QTcpSocket>
#include "datasource.h"
#include <QDateTime>

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

//    connect(&m_delayTimer, &QTimer::timeout, this, &NetworkModel::delayTimeout);
    connect(&m_timeoutTimer, &QTimer::timeout, this, &NetworkModel::sendTimeout);

//    m_delayTimer.setSingleShot(true);
    m_timeoutTimer.setSingleShot(true);
}

NetworkModel::~NetworkModel() {
    clear();
}
// controller to model interface overrides


void NetworkModel::setDelay(int delay) {
    m_delayMs = delay;
}

void NetworkModel::setTimeout(int timeout) {
    m_timeoutMs = timeout;
}


void NetworkModel::start(DataSource& networkDevice)
{
    //m_facade.setBaudRates(m_deviceModelFactory.getBaudrate());

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
    for(quint8 iAddr = 1; iAddr <= SoftProtocol::MaxAddress; ++iAddr) {
        m_queue.enqueue(m_protocol.getDataValue(iAddr, NetworkModel::IDENTIFY_REG_ID_DEFAULT));
    }
    tryToSend();
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
    connect(newDevice.get(), SIGNAL(dataToModel(quint8,quint16,quint16)), this, SLOT(dataOutcome(quint8,quint16,quint16)));

    m_facade.createWidgetFor(newDevice.get());
}

void NetworkModel::tryToSend() {
    if(!m_portIsBusy) {
        m_portIsBusy = true;
        delayTimeout(); // try to send data immediately
//        m_delayTimer.setInterval(m_delayMs);
//        m_delayTimer.start();
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
//        device.
        // TODO: complete function's code
    }
}

// private slots
void NetworkModel::readyRead() {
    QByteArray rxPacket = m_port->readAll();
//    qDebug() << "RX" << rxPacket.toHex(' ');
    m_timeoutTimer.stop();
    SoftProtocol::DataVector result = m_protocol.execute(rxPacket, m_lastTxPackage);

    m_lastTxPackage.clear();
    for(const auto& item : qAsConst(result)) {
        if(item.reg == NetworkModel::IDENTIFY_REG_ID_DEFAULT) {
            initDevice(item.addr, item.value);
        } else {
            if(m_devices.contains(item.addr)) {
                m_devices[item.addr]->dataIncome(item.reg, item.value);
            }
        }
    }
    m_portIsBusy = false;
    tryToSend();
}

void NetworkModel::bytesWritten(qint64 bytes) {
    m_bytesWritten += bytes;
    if(m_bytesWritten >= m_lastTxPackage.size()) {
//        qDebug() << "Written bytes";
        m_bytesWritten = 0;
        if(m_protocol.needWaitForAnswer(m_lastTxPackage)) {
            m_timeoutTimer.setInterval(m_timeoutMs);
            m_timeoutTimer.start();
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
    m_lastTxPackage.clear();
//    qDebug() << "Timeout";
    tryToSend();
}

void NetworkModel::delayTimeout() {
//    m_delayTimer.stop();
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
            //            tryToSend();
            delayTimeout();
            return;
        }

        m_port->write(m_lastTxPackage);
//        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "Write! " << m_lastTxPackage.toHex(' ');
    }
}
// =====================


