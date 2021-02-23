#include "mainviewfacade.h"
#include "globals.h"
#include <QDebug>
#include <QSerialPort>
#include <QTcpSocket>
#include <QHostAddress>

MainViewFacade::MainViewFacade(QObject *parent) : QObject(parent), ViewInterface(), MainWindowControllerInterface()
{

}

// View Interface

void MainViewFacade::clearDevices() {
    m_devices.clear();
}

void MainViewFacade::addDevice(quint8 addr, quint16 id) {
    if(addr == 0 || addr > MAX_DEVICES) return;

    DeviceModel* model = nullptr;
    if(m_devices[addr] != nullptr) {
        delete m_devices[addr];
    }
//    model = ;// dev factory
    m_devices[addr] = model;
}

void MainViewFacade::removeDevice(quint8 addr) {
    if(addr == 0 || addr > MAX_DEVICES) return;

    if(m_devices[addr] != nullptr) {
        delete m_devices[addr];
        m_devices[addr] = nullptr;
    }
}

bool MainViewFacade::hasDevice(quint8 addr) {
    return (m_devices[addr] != nullptr);
}

quint16 MainViewFacade::deviceId(quint8 addr) {
    DeviceModel* model = m_devices[addr];
    if(model != nullptr) {

    }
    return 0;
}

void MainViewFacade::setDeviceData(quint8 addr, quint16 cmd, quint16 value) {
    if(hasDevice(addr)) {
//        m_devices[addr]->
        qDebug() << "[Facade] setDeviceData(" << addr << ", " << cmd << ", " << value << ")";
    }
}

//void MainViewFacade::setDeviceLink(quint8 addr, bool link) {
//    DeviceModel* model = nullptr;
//    model = m_devices.at(addr);
//}

// MainWindowController Interface
void MainViewFacade::addModel(ModelInterface* model) {
    m_models.removeAll(model);
    m_models.append(model);
}

void MainViewFacade::removeModel(ModelInterface* model) {
    m_models.removeAll(model);
}

void MainViewFacade::clearModels() {
    m_models.clear();
}

void MainViewFacade::networkConnectClicked(CONNECT_PROTOCOL protocol, QString host, int port) {
    QIODevice* IODevice = nullptr;

    if(m_models.isEmpty()) {
        qDebug() << "[DEBUG](MainViewFacade) models list is empty";
        return;
    }

    QTcpSocket* tcpSocket = nullptr;
    QSerialPort* serialPort = nullptr;

    switch(protocol) {
    case TCP_PROTOCOL:
        tcpSocket = new QTcpSocket();
        tcpSocket->connectToHost(QHostAddress(host), port, QIODevice::ReadWrite);
        for (ModelInterface* model : m_models) {
            if(!model->isStart()) {
                model->start(tcpSocket);
            }
        }
//        emit connected(tcpSocket->isOpen());
        break;
    case COM_PORT_PROTOCOL:
        serialPort = new QSerialPort();
        serialPort->setPortName(host);
        serialPort->setBaudRate(port);
        serialPort->open(QIODevice::ReadWrite);
//        emit connected(serialPort->isOpen());
        for (ModelInterface* model : m_models) {
            if(!model->isStart()) {
                model->start(tcpSocket);
            }
        }
        break;
    case UNKNOWN_PROTOCOL:
    default:
        qDebug() << "[DEBUG](MainViewFacade) unknown protocol";
        return;
        break;
    }

    for(ModelInterface* model : m_models ) {
        if(model->isStart()) model->stop();
    }
}
