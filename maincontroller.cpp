#include "maincontroller.h"
#include "appsettings.h"
#include <QSerialPortInfo>

MainController::MainController(MainWindow& window, NetworkModel& networkModel, QObject *parent) :
    QObject(parent),
    window(window),
    network(networkModel)
{
    connect(&window, &MainWindow::connectToNetwork, this, &MainController::connectToNetwork);
    connect(&window, &MainWindow::refreshComPortsSignal, this, &MainController::refreshComPorts);
}


// private slots

void MainController::refreshComPorts() {
    QStringList ports;
    for(const auto& port : QSerialPortInfo::availablePorts()) {
        ports << port.portName();
    }
    window.setComPorts(ports);
}

void MainController::connectToNetwork(QVariant value) {
    AppSettings::setNetworkData(value);

    QVariantMap portSettings = value.toMap();

    if(network.isStart()) {
        network.stop();
        device->close();
        window.setConnected(device->isOpen());
//            device->deleteLater();
    } else {
        NetworkType type = static_cast<NetworkType>(portSettings["type"].toInt());
        if(type == NetworkType::Tcp) {
            device = new DataSource;
            device->setSettings(type, portSettings["host"], portSettings["port"]);
            device->open();
            window.setConnected(device->isOpen());
            network.start(*device);
        } else if(type == NetworkType::SerialPort) {
            // none. do it!
        }
    }
}
