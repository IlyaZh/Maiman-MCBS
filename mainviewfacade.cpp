#include "mainviewfacade.h"
#include "globals.h"
#include <QDebug>
#include <QSerialPort>
#include <QTcpSocket>
#include <QHostAddress>
#include "model/device/devicewidget.h"

MainViewFacade::MainViewFacade(QObject *parent) : QObject(parent), ViewInterface()//, MainWindowControllerInterface()
{
    m_mainWindow = nullptr;
}

// View Interface

void MainViewFacade::createdDevice(Device* pDev) {
    if(!m_devices.contains(pDev->addr())) {
        m_devices.insert(pDev->addr(), pDev);
        qDebug() << "Facade created device with addr = " << pDev->addr();

        // make a device widget and link it with device;
    }
}

void MainViewFacade::removeDevice(Device* pDev) {
    m_devices.remove(pDev->addr());
}

void MainViewFacade::removeAllDevices() {
    m_devices.clear();
}

/*void MainViewFacade::clearDevices() {
    m_devices.clear();
}

void MainViewFacade::addDevice(quint8 addr, const Device* dev) {
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
    qDebug() << "setDeviceData(addr=" << addr << "cmd=" << cmd << "value=" << value << ")";
    if(hasDevice(addr)) {
//        m_devices[addr]->
        qDebug() << "[Facade] setDeviceData(" << addr << ", " << cmd << ", " << value << ")";
    }
}*/

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

void MainViewFacade::addView(QMainWindow* mainWindow) {
    if(m_mainWindow != nullptr) m_mainWindow->disconnect();

    m_mainWindow = mainWindow;
//    m_mainWindow->addController(this);
    connect(this, SIGNAL(connected(bool)), m_mainWindow, SLOT(setConnected(bool)));
    connect(m_mainWindow, SIGNAL(networkConnectClicked(int, QString, int)), this, SLOT(networkConnectClicked(int, QString, int)));

}

void MainViewFacade::networkConnectClicked(int protocol, QString host, int port) {
    if(m_models.isEmpty()) {
        qDebug() << "[DEBUG](MainViewFacade) models list is empty";
        return;
    }
    qDebug() << protocol << host << port; //

    QTcpSocket* tcpSocket = nullptr;
    QSerialPort* serialPort = nullptr;

    for (ModelInterface* model : m_models) {
        if(model->isStart()) {
            model->stop();
            emit connected(false);
        } else {
            switch(protocol) {
            case TCP_PROTOCOL:
                if(tcpSocket == nullptr) {
                    tcpSocket = new QTcpSocket();
                    tcpSocket->connectToHost(host, port, QIODevice::ReadWrite);
                    emit connected(tcpSocket->isOpen());
                }
                model->start(tcpSocket);
                break;
            case COM_PORT_PROTOCOL:
                if(serialPort == nullptr) {
                    serialPort = new QSerialPort();
                    serialPort->setPortName(host);
                    serialPort->setBaudRate(port);
                    serialPort->open(QIODevice::ReadWrite);
                    emit connected(serialPort->isOpen());
                }
                model->start(serialPort);
                break;
            case UNKNOWN_PROTOCOL:
            default:
                qDebug() << "[DEBUG](MainViewFacade) unknown protocol";
//                return;
                break;
            }
        }
    }



}
