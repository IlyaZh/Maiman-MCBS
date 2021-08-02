#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <QObject>
#include <QPointer>
#include <QIODevice>
#include <QVector>
#include "protocols/modbus.h"
#include "interfaces/ProtocolObserverInterface.h"
#include "model/ModelInterface.h"
#include "SoftProtocol.h"
#include "device/devicemodel.h"
#include "model/devicefactory.h"
#include "mainviewfacade.h"
#include "windownetworkmediator.h"
#include <QPointer>

//#include "enums.h"

class MainViewFacade;

class NetworkModel :
        public QObject,
        public ProtocolObserverInterface,
        public ModelInterface,
        public BaseComponent
{
    Q_OBJECT
public:
    static const quint16 IDENTIFY_REG_ID_DEFAULT;
    static const quint16 TIMEOUT_MS;
    explicit NetworkModel(DeviceFactory &deviceModelFactory, SoftProtocol& protocol, QObject *parent = nullptr);
    ~NetworkModel();
    void start(DataSourceInterface &iodevice) override;
    bool isStart() override;
    void stop() override;
//    void setDeviceCommand(quint8 addr, quint16 command, quint16 value) override;
    void rescanNetwork() override;
    void addFacade(MainViewFacade &facade);
    void timeout(quint8 code) override;


signals:
    void modelConnected(bool);

public slots:
    void dataIncome(quint8 addr, quint16 reg, quint16 value) override;
    void dataReady() override;
    void dataOutcome(quint8 addr, quint16 reg, quint16 value);
    void errorOccured(QString msg) override;
    void deviceOpen(bool state) override;
private slots:


private:
    DeviceFactory& m_deviceModelFactory;
    QPointer<MainViewFacade> m_view;
    SoftProtocol& m_netDevice;
    QMap<quint8, Device*> m_devices;
    bool m_bIsStart;
//    bool m_pollEnabled;


    void clear();
    void initDevice(quint8 addr, quint16 id);
    void pollDevices();
};


#endif // NETWORKMODEL_H
