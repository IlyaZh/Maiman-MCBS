#include "mainviewfacade.h"
#include "globals.h"
#include <QDebug>
#include <QSerialPort>
#include <QTcpSocket>
#include <QHostAddress>
#include "model/device/devicewidget.h"

MainViewFacade::MainViewFacade(DataSourceInterface &dataSource, AppSettings &settings, GuiFactory& factory, QObject *parent) :
    QObject(parent), ViewInterface(), //MainWindowControllerInterface(),
    m_factory(factory),
    m_dataSource(dataSource),
    m_settings(settings)
{
    m_mainWindow = nullptr;

    m_factory.start();

    // uncomment
    connect(&m_dataSource, &DataSourceInterface::errorOccured, [=](QString msg){
        m_mainWindow->setConnectMessage(msg);
    });

//    pGuiFactory->start();
}

// View Interface

void MainViewFacade::createdDevice(Device* pDev) {
    if(!m_devices.contains(pDev->addr())) {
        m_devices.insert(pDev->addr(), pDev);
        qDebug() << "Facade created device with addr = " << pDev->addr();

        // make a device widget and link it with device;
//        QWidget* deviceWidget = m_guiFactory->createWidget(pDev);

        // attach deviceWidget to main window
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
//    m_models.removeAll(model);
//    m_models.append(model);
    m_models = model;
}

void MainViewFacade::removeModel(ModelInterface* model) {
//    m_models.removeAll(model);
    m_models = nullptr;
}

void MainViewFacade::clearModels() {
//    m_models.clear();
    m_models = nullptr;
}

void MainViewFacade::addView(MainWindow* mainWindow) {
    if(m_mainWindow != nullptr) m_mainWindow->disconnect();

    m_mainWindow = mainWindow;
//    m_mainWindow->addController(this);
    connect(this, SIGNAL(connected(bool)), m_mainWindow, SLOT(setConnected(bool)));
    connect(m_mainWindow, SIGNAL(networkConnectClicked(NetworkType, QVariant, QVariant)), this, SLOT(networkConnectClicked(NetworkType, QVariant, QVariant)));

}

void MainViewFacade::networkConnectClicked(NetworkType protocol, QVariant host, QVariant port) {
    if(m_models == nullptr) {
        qDebug() << "[DEBUG](MainViewFacade) models list is empty";
        return;
    }
    qDebug() << static_cast<uint>(protocol) << host << port; //

    if(m_models->isStart()) {
        if(m_dataSource.isOpen()) m_dataSource.close();
        m_models->stop();
    } else {
        m_dataSource.setSettings(protocol, host, port);
        if(m_dataSource.open())
            m_models->start(m_dataSource);

    }
    qDebug() << "emit connected = " << m_dataSource.isOpen();
    emit connected(m_dataSource.isOpen());

//    QTcpSocket* tcpSocket = nullptr;
//    QSerialPort* serialPort = nullptr;

    /*for (ModelInterface* model : m_models) {
        qDebug() << "networkConnectClicked" << model->isStart();
        if(model->isStart()) {
            model->stop();
            emit connected(false);
            qDebug() << "emit connected = false";
        } else {

            if(protocol == NetworkType::TCP) {
                tcpSocket = new QTcpSocket();
                tcpSocket->connectToHost(host.toString(), port.toUInt(), QIODevice::ReadWrite);
                emit connected(tcpSocket->isOpen());
                qDebug() << "emit connected = " << tcpSocket->isOpen();
                model->start(tcpSocket);
            } else if (protocol == NetworkType::COM_PORT) {
                serialPort = new QSerialPort();
                serialPort->setPortName(host.toString());
                serialPort->setBaudRate(port.toInt());
                serialPort->open(QIODevice::ReadWrite);
                emit connected(serialPort->isOpen());
                model->start(serialPort);
            } else {
                qDebug() << "[DEBUG](MainViewFacade) unknown protocol";
            }
        }
    }*/



}
