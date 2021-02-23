#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include "network/networkmodel.h"
#include <QVector>
#include "enums.h"

class MainWindowControllerInterface
{
public:
    MainWindowControllerInterface() { }
    virtual void addModel(ModelInterface* model) = 0;
    virtual void removeModel(ModelInterface* model) = 0;
    virtual void clearModels() = 0;

    virtual void networkConnectClicked(CONNECT_PROTOCOL protocol, QString host, int port) = 0;

protected:
    QVector<ModelInterface*> m_models;

};

#endif // MAINWINDOWCONTROLLER_H
