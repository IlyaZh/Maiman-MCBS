#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <QtWidgets>
//#include <QObject>
//#include <QPointer>
//#include <QIODevice>
//#include <QVector>
#include "protocols/modbusprotocol.h"
#include "interfaces/ProtocolObserverInterface.h"
#include "SoftProtocol.h"
#include "device/devicemodel.h"
#include "model/devicefactory.h"
//#include <QPointer>
#include "mainfacade.h"
#include "datasource.h"
#include "constants.h"
//#include <QQueue>
//#include <QTimer>
//#include <QSharedPointer>

//#include "enums.h"

class MainViewFacade;

class NetworkModel :
        public QObject
{
    Q_OBJECT
public:
    static const quint16 IDENTIFY_REG_ID_DEFAULT;
    static const quint16 TIMEOUT_MS;
    explicit NetworkModel(DeviceFactory &deviceModelFactory, SoftProtocol& protocol, MainFacade& facade, QObject *parent = nullptr);
    ~NetworkModel();
    void setDelay(int delay);
    void setTimeout(int timeout);
    void start(DataSource &iodevice);
    bool isStart();
    void stop();
    void rescanNetwork();

public slots:
    void dataOutcome(quint8 addr, quint16 reg, quint16 value);
    void temperatureUnitsChanged(Const::TemperatureUnitId id);
private slots:
    void readyRead();
    void bytesWritten(qint64 bytes);
    void errorOccured(const QString& msg);
    void sendTimeout();
    void delayTimeout();


private:
    DeviceFactory& m_deviceModelFactory;
    QScopedPointer<DataSource> m_port;
    MainFacade& m_facade;
    SoftProtocol& m_protocol;
    QMap<quint8, QSharedPointer<Device>> m_devices;
    bool m_bIsStart = false;
    bool m_portIsBusy = false;
    QQueue<QByteArray> m_queue;
    QQueue<QByteArray> m_priorityQueue;
    qint64 m_bytesWritten;
    QByteArray m_lastTxPackage;
    QTimer m_timeoutTimer;
    QTimer m_delayTimer;
    int m_timeoutMs = 300;
    int m_delayMs = 0;

    void clear();
    void initDevice(quint8 addr, quint16 id);
    void tryToSend();
};


#endif // NETWORKMODEL_H
