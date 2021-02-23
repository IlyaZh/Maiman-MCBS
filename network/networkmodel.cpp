#include "networkmodel.h"
#include <QDebug>

const quint16 NetworkModel::TIMEOUT_MS = 50*10;
const quint16 NetworkModel::IDENTIFY_REG_ID_DEFAULT = 0x0702; // debug замени

NetworkModel::NetworkModel(SoftProtocol* protocol, QObject *parent) : QObject(parent), ProtocolObserverInterface(), ModelInterface()
{
    m_bIsStart = false;
    m_netDevice = protocol;
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

    // parse a device model file

}

bool NetworkModel::isStart() {
    return m_bIsStart;
}

void NetworkModel::stop()
{
   m_netDevice->removeObserver(this);
   m_netDevice->stop();
   m_bIsStart = false;
}

void NetworkModel::setDeviceCommand(quint8 addr, quint16 command, quint16 value)
{
    m_netDevice->setDataValue(addr, command, value);
}

void NetworkModel::rescanNetwork()
{
    clear();
    for(quint8 iAddr = 1; iAddr <= SoftProtocol::MAX_ADDRESS; ++iAddr) {
        m_netDevice->getDataValue(iAddr, NetworkModel::IDENTIFY_REG_ID_DEFAULT);
    }
}

// modbus interface overrides

void NetworkModel::dataNotify(quint8 addr, quint16 reg, quint16 value)
{
    if(reg == NetworkModel::IDENTIFY_REG_ID_DEFAULT) initDevice(addr, value);
    dataReady();
}

void NetworkModel::dataReady()
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
    qDebug() << "Init device " << addr << " with id " << id;
}
