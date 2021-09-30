#include "maincontroller.h"

MainController::MainController(MainWindow& window, NetworkModel& networkModel, AppSettings& settings, QObject *parent) :
    QObject(parent),
    window(window),
    network(networkModel),
    settings(settings)
{
    connect(&window, &MainWindow::makeEvent, this, &MainController::eventHandle);
}


// private slots

void MainController::eventHandle(const QString &event, const QVariant &value) {
    if(event == "NetworkConnectClicked") {
        settings.setNetworkData(value);

        QVariantMap portSettings = value.toMap();

        if(network.isStart()) {
            network.stop();
            device->close();
//            device->deleteLater();
        } else {
            NetworkType type = static_cast<NetworkType>(portSettings["type"].toInt());
            if(type == NetworkType::Tcp) {
                device = new DataSource();
                device->setSettings(type, portSettings["host"], portSettings["port"]);
                device->open();
                network.start(*device);
            } else if(type == NetworkType::SerialPort) {
                // none. do it!
            }
        }
        window.setConnected(device->isOpen());
    }
}
