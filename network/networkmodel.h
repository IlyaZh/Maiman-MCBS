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
#include <QPointer>
#include "mainfacade.h"
#include "datasource.h"
#include <QQueue>
#include <QTimer>

//#include "enums.h"

class MainViewFacade;

class NetworkModel :
        public QObject,
        public ModelInterface
{
    Q_OBJECT
public:
    static const quint16 IDENTIFY_REG_ID_DEFAULT;
    static const quint16 TIMEOUT_MS;
    explicit NetworkModel(DeviceFactory &deviceModelFactory, SoftProtocol& protocol, MainFacade& facade, QObject *parent = nullptr);
    ~NetworkModel();
    void setDelay(int delay) override;
    void setTimeout(int timeout) override;
    void start(IDataSource &iodevice) override;
    bool isStart() override;
    void stop() override;
    void rescanNetwork() override;

public slots:
    void dataOutcome(quint8 addr, quint16 reg, quint16 value);
private slots:
    void readyRead();
    void bytesWritten(qint64 bytes);
    void errorOccured(const QString& msg);
    void sendTimeout();
    void delayTimeout();


private:
    DeviceFactory& m_deviceModelFactory;
    QPointer<IDataSource> m_port;
    MainFacade& m_facade;
    SoftProtocol& m_protocol;
    QMap<quint8, Device*> m_devices;
    bool m_bIsStart = false;
    bool m_portIsBusy = false;
    QQueue<QByteArray> m_queue;
    QQueue<QByteArray> m_priorityQueue;
    qint64 m_bytesWritten;
    QByteArray m_lastTxPackage;
    QTimer m_timeoutTimer;
    QTimer m_delayTimer;
    int m_timeoutMs = 1000;
    int m_delayMs = 50;

    void clear();
    void initDevice(quint8 addr, quint16 id);
    void tryToSend();
};


#endif // NETWORKMODEL_H
