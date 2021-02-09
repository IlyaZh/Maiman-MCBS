#include "networkmodel.h"

const quint16 NetworkModel::TIMEOUT_MS = 50;
const quint16 NetworkModel::IDENTIFY_REG_ID_DEFAULT = 0702;

NetworkModel::NetworkModel(QObject *parent) : QObject(parent), ModbusObserverInterface(), NetworkModelInInterface()
{

}

NetworkModel::~NetworkModel() {
    clear();
}

// controller to model interface overrides

void NetworkModel::start(QIODevice *networkDevice)
{
    m_netDevice = new Modbus(networkDevice, TIMEOUT_MS, this);
    m_netDevice->addObserver(this);

    rescanNetwork();

    // parse a device model file

}

void NetworkModel::stop()
{
   m_netDevice->removeObserver(this);
   m_netDevice.clear();
}

void NetworkModel::setDeviceCommand(quint8 addr, quint16 command, quint16 value)
{
    m_netDevice->setDataValue(addr, command, value);
}

void NetworkModel::rescanNetwork()
{
    clear();
    for(quint8 iAddr = 1; iAddr <= Modbus::MAX_ADDRESS; ++iAddr) {
        m_netDevice->getDataValue(iAddr, NetworkModel::IDENTIFY_REG_ID_DEFAULT);
    }
}

// modbus interface overrides

void NetworkModel::modbusNotify(quint8 addr, quint16 reg, quint16 value)
{
    if(reg == NetworkModel::IDENTIFY_REG_ID_DEFAULT) initDevice(addr, value);
    modbusReady();
}

void NetworkModel::modbusReady()
{
    // request a new value
}

// private methods
void NetworkModel::clear() {
    for(auto *item : m_devices) {
        delete item;
    }
    m_devices.clear();
}

void NetworkModel::initDevice(quint8 addr, quint16 id)
{

}
