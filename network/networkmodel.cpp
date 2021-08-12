#include "networkmodel.h"
#include <QDebug>
#include "model/device/devicepollrequest.h"
#include <QTcpSocket>

const quint16 NetworkModel::TIMEOUT_MS = 50*10;
const quint16 NetworkModel::IDENTIFY_REG_ID_DEFAULT = 0x0001; // debug замени

NetworkModel::NetworkModel(DeviceFactory &deviceModelFactory, SoftProtocol& protocol, MainFacade& facade, QObject *parent) :
    QObject(parent),
    ModelInterface(),
    m_deviceModelFactory(deviceModelFactory),
    m_facade(facade),
    m_protocol(protocol)
{
    m_bIsStart = false;
    m_deviceModelFactory.start();

    m_protocol.Attach(this);
}

NetworkModel::~NetworkModel() {
    clear();
}

// controller to model interface overrides

void NetworkModel::start(IDataSource& networkDevice)
{
    m_port = &networkDevice;
    connect(m_port, &IDataSource::errorOccured, this, [=](QString msg){
        qDebug() << "NetworkModel error " << msg;
    });
    connect(m_port, &IDataSource::deviceOpen, this, [=](bool state){
        qDebug() << "NetworkModel protocol state" << state;
    });


    m_protocol.setDevice(networkDevice);

    m_bIsStart = true;

    rescanNetwork();
}

bool NetworkModel::isStart() {
    return m_bIsStart;
}

void NetworkModel::stop()
{
    qDebug() << "NetworkModel::stop()";
   m_bIsStart = false;
   clear();
}

//void NetworkModel::setDeviceCommand(quint8 addr, quint16 command, quint16 value)
//{
//    m_netDevice->setDataValue(addr, command, value);
//}

void NetworkModel::rescanNetwork()
{
    qDebug() << "rescanNetwork()";
    clear();
    for(quint8 iAddr = 1; iAddr <= SoftProtocol::MaxAddress; ++iAddr) {
        m_protocol.getDataValue(iAddr, NetworkModel::IDENTIFY_REG_ID_DEFAULT);
    }
//    m_mediator->notify(this, "remove all devices");
    m_facade.clear();
}

//void NetworkModel::addFacade(MainViewFacade &facade) {
//    m_view = &facade;
//    m_view->addModel(this);
//}

// private methods
void NetworkModel::clear() {
//    m_pollEnabled = false;
    qDebug() << "NetworkModel::clear(). Delete devices";

    for(auto item : m_devices) {
        item->disconnect();
        item->deleteLater();
    }
    qDebug() << "m_devices=" << m_devices.size();
    m_devices.clear();

}

void NetworkModel::initDevice(quint8 addr, quint16 id)
{
    qDebug() << "Init device " << addr << " with id " << id;
    Device* newDevice =  m_deviceModelFactory.createDevice(addr, id);
    if(newDevice == nullptr) {
        qDebug() << "Device with id " << id << "hasn't found in config";
        return;
    }
    else
        qDebug() << newDevice->name();

    if(m_devices.contains(addr)) {
        auto oldDev = m_devices.value(addr);
        oldDev->disconnect();
        oldDev->deleteLater();
        m_devices.remove(addr);
        qDebug() << "initDevice duplicate device addr";
    }

    m_devices.insert(addr, newDevice);
    connect(newDevice, SIGNAL(dataToModel(quint8,quint16,quint16)), this, SLOT(dataOutcome(quint8,quint16,quint16)));

    m_facade.createWidgetFor(*newDevice);
}

// public slots
// modbus interface overrides

void NetworkModel::update(quint8 addr, quint16 reg, quint16 value)
{
    qDebug() << "READ" << addr << reg << value;
    if(reg == NetworkModel::IDENTIFY_REG_ID_DEFAULT) {
        initDevice(addr, value);
    } else {
        if(m_devices.contains(addr)) {
            m_devices[addr]->dataIncome(reg, value);
        }
    }
}

void NetworkModel::iamReady() {
    // poll devices state
    for(auto dev : m_devices) {
        DevicePollRequest* request = dev->nextPollRequest();
        if(request != nullptr) {
            m_protocol.getDataValue(request->addr(), request->code(), request->count());
        }
    }
}

/*void NetworkModel::dataReady()
{
    qDebug() << "m_bIsStart" << m_bIsStart;
        // request a new value
        pollDevices();
}*/

void NetworkModel::dataOutcome(quint8 addr, quint16 reg, quint16 value)
{
    m_protocol.setDataValue(addr, reg, value);
}

/*void NetworkModel::timeout(quint8 code) {
//    if(m_devices.contains(code)) {
//        m_devices[code]->clearLink();
//        qDebug() << "Timeout code" << code;
//    }
}

void NetworkModel::errorOccured(QString msg) {
    qDebug() << QString("Device error %1").arg(msg);
}

void NetworkModel::deviceOpen(bool state) {
    qDebug() << "Device state = " << state;
}*/
// =====================


