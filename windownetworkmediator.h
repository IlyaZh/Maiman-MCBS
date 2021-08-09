#ifndef WINDOWNETWORKMEDIATOR_H
#define WINDOWNETWORKMEDIATOR_H

#include <QObject>
#include "interfaces/IMediator.h"
#include <QDebug>
#include "appsettings.h"
#include "mainwindow.h"
#include "network/networkmodel.h"

class NetworkModel;

class WindowNetworkMediator : public IMediator
{
public:
    WindowNetworkMediator(MainWindow& window, NetworkModel& network, AppSettings& settings);
    ~WindowNetworkMediator();
    void notify(IMediatorBase *sender, QString event, QVariant value = QVariant()) override;
private:
    MainWindow& window;
    NetworkModel& network;
    AppSettings& settings;
    QPointer<DataSource> device;
};

#endif // WINDOWNETWORKMEDIATOR_H
