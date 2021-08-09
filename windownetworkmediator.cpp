#include "windownetworkmediator.h"

WindowNetworkMediator::WindowNetworkMediator(MainWindow& window, NetworkModel& network, AppSettings& settings) :
    window(window),
    network(network),
    settings(settings)

{
    window.setMediator(this);
    network.setMediator(this);

}

WindowNetworkMediator::~WindowNetworkMediator() {
    if(device) device->deleteLater();
}

void WindowNetworkMediator::notify(IMediatorBase *sender, QString event, QVariant value) {
    if(sender == &window && event == "NetworkConnectClicked") {
        settings.setNetworkData(value);

        QVariantMap portSettings = value.toMap();

        if(network.isStart()) {
            network.stop();
            device->close();
            device->deleteLater();
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
