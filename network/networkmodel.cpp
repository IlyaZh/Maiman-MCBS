#include "networkmodel.h"

const quint16 NetworkModel::TIMEOUT_MS = 50*10;
const quint16 NetworkModel::IDENTIFY_REG_ID_DEFAULT = 0x0702; // debug замени

NetworkModel::NetworkModel(QObject *parent) : QObject(parent), ModbusObserverInterface(), ModelInterface()
{
    m_bIsStart = false;
}

NetworkModel::~NetworkModel() {
    clear();
}

// controller to model interface overrides

void NetworkModel::start(CONNECT_PROTOCOL protocol, QString host, int port)
{
    if (protocol == UNKNOWN_PROTOCOL || host.isEmpty() || port == 0) {
        return;
    }

    QIODevice *networkDevice = nullptr;

    switch (protocol) {
    case COM_PORT_PROTOCOL:

        break;
    case TCP_PROTOCOL:
        break;
    case UNKNOWN_PROTOCOL:
    default:
        return;
    }

    m_netDevice = new Modbus(networkDevice, TIMEOUT_MS, this);
    m_netDevice->addObserver(this);
    m_bIsStart = true;

    rescanNetwork();

    // parse a device model file

}

bool NetworkModel::isStart() {
    return m_bIsStart;
}

void NetworkModel::stop()
{
   m_netDevice->removeObserver(this);
   m_netDevice->stop();
   m_netDevice.clear();
   m_bIsStart = false;
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
