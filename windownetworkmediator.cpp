#include "windownetworkmediator.h"

WindowNetworkMediator::WindowNetworkMediator(BaseComponent* window, BaseComponent* network, AppSettings& settings) :
    window(window),
    network(network),
    settings(settings)

{
    window->setMediator(this);
    network->setMediator(this);

}

void WindowNetworkMediator::Notify(BaseComponent *sender, QString event, QVariant value) const {
    if(sender == window && event == "NetworkConnectClicked") {
        settings.setNetworkData(value);
        qDebug() << value;
    }
}
