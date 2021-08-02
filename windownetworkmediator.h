#ifndef WINDOWNETWORKMEDIATOR_H
#define WINDOWNETWORKMEDIATOR_H

#include <QObject>
#include "MediatorPattern.h"
#include <QDebug>
#include "appsettings.h"

class WindowNetworkMediator : public Mediator
{
public:
    WindowNetworkMediator(BaseComponent* window, BaseComponent* network, AppSettings& settings);
    void Notify(BaseComponent *sender, QString event, QVariant value = QVariant()) const override;
private:
    BaseComponent* window;
    BaseComponent* network;
    AppSettings& settings;
};

#endif // WINDOWNETWORKMEDIATOR_H
