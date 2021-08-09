#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <QObject>
#include <QPointer>
#include <QIODevice>
#include <QVector>
#include "protocols/modbusprotocol.h"
#include "interfaces/ProtocolObserverInterface.h"
#include "model/ModelInterface.h"
#include "SoftProtocol.h"
#include "device/devicemodel.h"
#include "model/devicefactory.h"
#include "windownetworkmediator.h"
#include <QPointer>
#include "interfaces/IMediator.h"

//#include "enums.h"

class MainViewFacade;

class NetworkModel :
        public QObject,
        public ISoftProtocolObserver,
        public ModelInterface,
        public IMediatorBase
{
    Q_OBJECT
public:
    static const quint16 IDENTIFY_REG_ID_DEFAULT;
    static const quint16 TIMEOUT_MS;
    explicit NetworkModel(DeviceFactory &deviceModelFactory, SoftProtocol& protocol, QObject *parent = nullptr);
    ~NetworkModel();
    void start(IDataSource &iodevice) override;
    bool isStart() override;
    void stop() override;
//    void setDeviceCommand(quint8 addr, quint16 command, quint16 value) override;
    void rescanNetwork() override;
//    void addFacade(MainViewFacade &facade);
    // ISoftProtocolObserver
    void update(quint8 addr, quint16 reg, quint16 value) override;
    void iamReady() override;
    // end of ISoftProtocolObserver


signals:
    void modelConnected(bool);

public slots:
    void dataOutcome(quint8 addr, quint16 reg, quint16 value);
private slots:


private:
    DeviceFactory& m_deviceModelFactory;
    QPointer<IDataSource> m_port;
    QPointer<MainViewFacade> m_view;
    SoftProtocol& m_protocol;
    QMap<quint8, Device*> m_devices;
    bool m_bIsStart;
//    bool m_pollEnabled;


    void clear();
    void initDevice(quint8 addr, quint16 id);
};


#endif // NETWORKMODEL_H
