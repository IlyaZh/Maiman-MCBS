#include "networkmodel.h"
#include <QDebug>
#include "model/device/devicepollrequest.h"

const quint16 NetworkModel::TIMEOUT_MS = 50*10;
const quint16 NetworkModel::IDENTIFY_REG_ID_DEFAULT = 0x0001; // debug замени

NetworkModel::NetworkModel(DeviceFactory *deviceModelFactory, SoftProtocol* protocol, QObject *parent) : QObject(parent), ProtocolObserverInterface(), ModelInterface()
{
    m_bIsStart = false;
    m_pollEnabled = false;
    m_netDevice = protocol;
    m_deviceModelFactory = deviceModelFactory;
    m_deviceModelFactory->start();

//    m_view = vi;
//    m_view->addModel(this);
}

NetworkModel::~NetworkModel() {
    clear();
}

// controller to model interface overrides

void NetworkModel::start(QIODevice* networkDevice)
{
    if(networkDevice == nullptr) return;

    m_netDevice->setDevice(networkDevice);
    m_netDevice->addObserver(this);
    m_bIsStart = true;

    rescanNetwork();

    m_pollEnabled = true;
    pollDevices();
}

bool NetworkModel::isStart() {
    return m_bIsStart;
}

void NetworkModel::stop()
{
   m_netDevice->removeObserver(this);
   m_netDevice->stop();
   m_bIsStart = false;
   m_pollEnabled = false;
   clear();
}

//void NetworkModel::setDeviceCommand(quint8 addr, quint16 command, quint16 value)
//{
//    m_netDevice->setDataValue(addr, command, value);
//}

void NetworkModel::rescanNetwork()
{
    clear();
    for(quint8 iAddr = 1; iAddr <= SoftProtocol::MAX_ADDRESS; ++iAddr) {
        m_netDevice->getDataValue(iAddr, NetworkModel::IDENTIFY_REG_ID_DEFAULT);
    }
    if(m_view != nullptr) m_view->removeAllDevices();

}

void NetworkModel::addFacade(MainViewFacade* facade) {
    m_view = facade;
    m_view->addModel(this);
}

// private methods
void NetworkModel::clear() {
    m_pollEnabled = false;
    for(auto *item : m_devices) {
        delete item;
    }
    m_devices.clear();
}

void NetworkModel::initDevice(quint8 addr, quint16 id)
{
    qDebug() << "Init device " << addr << " with id " << id;
    Device* newDevice = m_deviceModelFactory->createDevice(addr, id);
    if(newDevice == nullptr) {
        qDebug() << "Device with id " << id << "hasn't found in config";
        return;
    }
    else
        qDebug() << newDevice->name();

    if(m_devices.contains(addr)) {
        Device* oldDev = m_devices.value(addr);
        oldDev->disconnect();
        oldDev->deleteLater();
    }

    m_devices.insert(addr, newDevice);
    connect(newDevice, &Device::dataToModel, this, &NetworkModel::dataOutcome);

    if(m_view != nullptr)
        m_view->createdDevice(newDevice);
}

void NetworkModel::pollDevices() {
    for(Device* dev : m_devices) {
        DevicePollRequest* request = dev->nextPollRequest();
        if(request != nullptr) {
            m_netDevice->getDataValue(request->addr(), request->code(), request->count());
        }
    }
}

// public slots
// modbus interface overrides

void NetworkModel::dataIncome(quint8 addr, quint16 reg, quint16 value)
{
    if(reg == NetworkModel::IDENTIFY_REG_ID_DEFAULT)
        initDevice(addr, value);
    else {
        if(m_devices.contains(addr)) {
            m_devices[addr]->dataIncome(reg, value);
        }
    }

    dataReady();
}

void NetworkModel::dataReady()
{
    // request a new value
    pollDevices();
}

void NetworkModel::dataOutcome(quint8 addr, quint16 reg, quint16 value)
{
    if(m_netDevice != nullptr) {
        m_netDevice->setDataValue(addr, reg, value);
    }
}

void NetworkModel::timeout(quint8 code) {
    if(m_devices.contains(code)) {
        m_devices[code]->clearLink();
    }
}

