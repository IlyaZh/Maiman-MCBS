#ifndef MAINVIEWFACADE_H
#define MAINVIEWFACADE_H

#include <QObject>
#include "interfaces/ViewInterface.h"
//#include "interfaces/mainwindowcontrollerinterface.h"
#include "mainwindow.h"
#include <QVector>
#include "model/ModelInterface.h"
#include "globals.h"
#include "appsettings.h"
#include "datasource.h"
#include "model/guifactory.h"

class ModelInterface;

class MainViewFacade : public QObject, public ViewInterface//, public MainWindowControllerInterface
{
    Q_OBJECT
public:
    explicit MainViewFacade(IDataSource& dataSource, AppSettings& settings, GuiFactory& factory, QObject *parent = nullptr);
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
    void addView(MainWindow* mainWindow);// override;

private slots:
    void networkConnectClicked(NetworkType protocol, QVariant host, QVariant port);// override;

signals:
    void connected(bool);

private:
//    QVector<ModelInterface*> m_models;
    GuiFactory& m_factory;
    ModelInterface* m_models;
    IDataSource& m_dataSource;
    MainWindow* m_mainWindow;
    QMap<quint16, Device*> m_devices;
    AppSettings& m_settings;

};

#endif // MAINVIEWFACADE_H
