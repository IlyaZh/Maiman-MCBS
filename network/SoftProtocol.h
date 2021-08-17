#ifndef SOFTPROTOCOLINTERFACE_H
#define SOFTPROTOCOLINTERFACE_H

#include <QObject>
//#include <QIODevice>
#include "datasource.h"
#include <QPointer>
#include <QTimer>
#include <QQueue>
#include "datasource.h"
#include "interfaces/ProtocolObserverInterface.h"


class ISoftProtocolObserver {
public:
    virtual ~ISoftProtocolObserver(){};
    virtual void update(quint8 addr, quint16 reg, quint16 value) = 0;
    virtual void iamReady() = 0;
    virtual void errorOccured(const QString& msg) = 0;
};

class ISoftProtocolSubject {
public:
    ~ISoftProtocolSubject(){};
    virtual void Attach(ISoftProtocolObserver* observer);
    virtual void Detach(ISoftProtocolObserver* observer);
protected:
    virtual void Notify(quint8 addr, quint16 reg, quint16 value);
    virtual void done();
    virtual void makeError(QString msg);
protected:
    QList<ISoftProtocolObserver*> m_listeners;
    QString m_errorString = "";
};

class SoftProtocol :
//        public QObject,
        public ISoftProtocolSubject
{
//    Q_OBJECT
public:
    static const quint8 MaxAddress;
    static const int TimeoutDefault;
    static const int DelayDefault;

    SoftProtocol(int delay_ms);
    virtual ~SoftProtocol() = default;
    void setDevice(IDataSource& device);
    virtual void setDataValue(quint8 addr, quint16 reg, quint16 value) = 0;
    virtual void getDataValue(quint8 addr, quint16 reg, quint8 count = 1) = 0;
    static quint8 hiBYTE(quint16 value);
    static quint8 loBYTE(quint16 value);

//signals:

//private slots:
    virtual void readyRead_Slot() = 0;
    virtual void bytesWritten_Slot(qint64 bytes) = 0;
//    virtual void CallbackBeforeSend() = 0;
protected:
    QTimer m_delayTmr;
    QPointer<IDataSource> m_device;
    bool bPortIsBusy = false;
    int m_bytesWritten;
    int m_delayMs;

    virtual void rxPacketHandler(const QByteArray &rxPacket) = 0;
    virtual void prepareAndWrite(QByteArray &byteArray) = 0;
    virtual void tryToSend();
    virtual void delayBeforeSend_Slot() = 0;
};



//class SoftProtocolOld : public QObject
//{
//    Q_OBJECT
//public:
//    static const quint8 MAX_ADDRESS;
//    static const int TIMEOUT_DEFAULT;
//    static const int DELAY_DEFAULT;
//    SoftProtocolOld(QString name, int timeoutMSecs = TIMEOUT_DEFAULT, int delayMSecs = DELAY_DEFAULT, QObject *parent = nullptr);
//    ~SoftProtocolOld();
//    virtual void start(IDataSource& device/*, int m_TimeoutMSecs = TIMEOUT_DEFAULT*/);
//    virtual void setTimeout(int) ;
//    virtual void setDelay(int);
////    virtual void setEnable(bool);
//    virtual bool isEnable();
//    virtual void addObserver(ProtocolObserverInterface* newObserver);
//    virtual void removeObserver(ProtocolObserverInterface* newObserver);
//    virtual void setDataValue(quint8 addr, quint16 reg, quint16 value);
//    virtual void getDataValue(quint8 addr, quint16 reg, quint8 count = 1);
//    virtual void stop();

//signals:
//    void errorOccured(QString);
//    void deviceOpen(bool);
////    void timeoutOccured(quint8 code);

//public slots:

//private slots:
//    virtual void readyRead() = 0;
//    virtual void bytesWritten(qint64 bytes);
//    virtual void timeout() = 0;
//    virtual void delayBeforeSendCallback();

//protected:
//    QByteArray m_lastTxPackage;
//    IDataSource* m_DataSource;
//    QQueue<QByteArray> m_Queue;
//    ProtocolObserverInterface* m_observer = nullptr;
//    QTimer m_timeoutTimer;
//    QTimer m_delayTimer;
//    int m_TimeoutMSecs;
//    int m_delayMSecs; // сделай установаку задержки
//    bool bPortIsBusy;
//    int m_bytesWritten;
//    QString m_name;

//    quint8 hiBYTE(quint16 value);
//    quint8 loBYTE(quint16 value);
//    virtual quint16 calcCrc(QByteArray &byteArray);
//    virtual void rxPacketHandler(QByteArray &rxPacket);
//    virtual void makeNotify(quint8 addr, quint16 reg, quint16 value);
//    virtual void prepareAndWrite(QByteArray &byteArray);
//    virtual void nothingToSend();
//    virtual void tryToSend();
//};

#endif // SOFTPROTOCOLINTERFACE_H
