#ifndef MAINVIEWFACADE_H
#define MAINVIEWFACADE_H

#include <QObject>
#include "interfaces/ViewInterface.h"
//#include "interfaces/mainwindowcontrollerinterface.h"
#include <QMainWindow>
#include <QVector>
#include "model/ModelInterface.h"
#include "enums.h"

class ModelInterface;

class MainViewFacade : public QObject, public ViewInterface//, public MainWindowControllerInterface
{
    Q_OBJECT
public:
    explicit MainViewFacade(QObject *parent = nullptr);
    // View Interface
    virtual void createdDevice( Device* pDev) override;
    virtual void removeDevice( Device* pDev) override;
    virtual void removeAllDevices() override;
    /*void clearDevices() override;
    void addDevice(quint8 addr, const Device* dev) override;
    void removeDevice(quint8 addr) override;
    bool hasDevice(quint8 addr) override;
    quint16 deviceId(quint8 addr) override;
    void setDeviceData(quint8 addr, quint16 cmd, quint16 value) override;*/
//    void setDeviceLink(quint8 addr, bool link) override;
    // MainWindowController Interface
    void addModel(ModelInterface* model);// override;
    void removeModel(ModelInterface* model);// override;
    void clearModels();// override;
    void addView(QMainWindow* mainWindow);// override;

private slots:
    void networkConnectClicked(int protocol, QString host, int port);// override;

signals:
    void connected(bool);

private:
    QVector<ModelInterface*> m_models;
    QMainWindow* m_mainWindow;
    QMap<quint16, Device*> m_devices;

};

#endif // MAINVIEWFACADE_H
