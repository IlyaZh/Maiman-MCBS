#ifndef MAINVIEWFACADE_H
#define MAINVIEWFACADE_H

#include <QObject>
#include "interfaces/ViewInterface.h"
#include "interfaces/mainwindowcontrollerinterface.h"
#include <QVector>

class MainViewFacade : public QObject, public ViewInterface, public MainWindowControllerInterface
{
    Q_OBJECT
public:
    explicit MainViewFacade(QObject *parent = nullptr);
    // View Interface
    void clearDevices() override;
    void addDevice(quint8 addr, quint16 id) override;
    void removeDevice(quint8 addr) override;
    bool hasDevice(quint8 addr) override;
    quint16 deviceId(quint8 addr) override;
    void setDeviceData(quint8 addr, quint16 cmd, quint16 value) override;
//    void setDeviceLink(quint8 addr, bool link) override;
    // MainWindowController Interface
    void addModel(ModelInterface* model) override;
    void removeModel(ModelInterface* model) override;
    void clearModels() override;
    void networkConnectClicked(CONNECT_PROTOCOL protocol, QString host, int port) override;

signals:

private:


};

#endif // MAINVIEWFACADE_H
