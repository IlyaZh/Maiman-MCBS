#include "maincontroller.h"
#include "appsettings.h"
#include <QSerialPortInfo>

MainController::MainController(MainWindow& window, NetworkModel& networkModel, QObject *parent) :
    QObject(parent),
    m_window(window),
    m_network(networkModel)
{
    connect(&window, &MainWindow::connectToNetwork, this, &MainController::connectToNetwork);
    connect(&window, &MainWindow::refreshComPortsSignal, this, &MainController::refreshComPorts);
    connect(&window, &MainWindow::tempratureUnitsChanged, &m_network, &NetworkModel::temperatureUnitsChanged);
}


// private slots

void MainController::refreshComPorts() {
    QStringList ports;
    const auto availPorts = QSerialPortInfo::availablePorts();
    for(const auto& port : availPorts) {
        ports << port.portName();
    }
    m_window.setComPorts(ports);
}

void MainController::connectToNetwork(QVariant value) {
    AppSettings::setNetworkData(value);

    QVariantMap portSettings = value.toMap();

    if(m_network.isStart()) {
        m_network.stop();
        m_device->close();
        m_window.setConnected(m_device->isOpen());
//            device->deleteLater();
    } else {
        NetworkType type = static_cast<NetworkType>(portSettings["type"].toInt());
        if(type == NetworkType::Tcp) {
            m_device = new DataSource;
            m_device->setSettings(type, portSettings["host"], portSettings["port"]);
            m_device->open();
            m_window.setConnected(m_device->isOpen());
            m_network.start(*m_device);
        } else if(type == NetworkType::SerialPort) {
            m_device = new DataSource;
            m_device->setSettings(type, portSettings["comport"], portSettings["baudrate"]);
            m_device->open();
            m_window.setConnected(m_device->isOpen());
            m_network.start(*m_device);
        }
    }
}
