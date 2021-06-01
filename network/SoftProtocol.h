#ifndef SOFTPROTOCOLINTERFACE_H
#define SOFTPROTOCOLINTERFACE_H

#include <QObject>
#include <QIODevice>
#include <QPointer>
#include <QTimer>
#include <QQueue>
#include "interfaces/ProtocolObserverInterface.h"

class SoftProtocol : public QObject
{
    Q_OBJECT
public:
    static const quint8 MAX_ADDRESS;
    static const int TIMEOUT_DEFAULT;
    SoftProtocol(QString name, QIODevice* device = nullptr, int timeoutMSecs = TIMEOUT_DEFAULT, QObject *parent = nullptr);
    ~SoftProtocol();
    virtual void setDevice(QIODevice* device, int m_TimeoutMSecs = TIMEOUT_DEFAULT);
    virtual void setTimeout(int) ;
//    virtual void setEnable(bool);
    virtual bool isEnable();
    virtual void addObserver(ProtocolObserverInterface* newObserver);
    virtual void removeObserver(ProtocolObserverInterface* newObserver);
    virtual void setDataValue(quint8 addr, quint16 reg, quint16 value);
    virtual void getDataValue(quint8 addr, quint16 reg, quint8 count = 1);
    virtual void stop();

signals:
    void errorOccured(QString);
//    void timeoutOccured(quint8 code);

public slots:

private slots:
    virtual void readyRead() = 0;
    virtual void bytesWritten(qint64 bytes);
    virtual void timeout() = 0;

protected:
    QByteArray *m_lastTxPackage;
    QIODevice* m_Device;
    QQueue<QByteArray*> m_Queue;
    QList<ProtocolObserverInterface*> m_vObservers;
    QTimer timeoutTimer;
    int m_TimeoutMSecs;
    bool bPortIsBusy;
    int m_bytesWritten;
    QString m_name;

    quint8 hiBYTE(quint16 value);
    quint8 loBYTE(quint16 value);
    virtual quint16 calcCrc(QByteArray *byteArray);
    virtual void prepareAndWrite(QByteArray* byteArray);
    virtual void rxPacketHandler(QByteArray *rxPacket);
    virtual void makeNotify(quint8 addr, quint16 reg, quint16 value);
    virtual void nothingToSend();
    virtual void tryToSend();
};

#endif // SOFTPROTOCOLINTERFACE_H
