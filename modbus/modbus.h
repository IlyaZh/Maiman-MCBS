#ifndef MODBUS_H
#define MODBUS_H

#include <QObject>
#include <QIODevice>
#include <QPointer>
#include <QTimer>
#include <QQueue>

#include "interfaces/modbusobserverinterface.h"

class Modbus : public QObject
{
    Q_OBJECT
public:
    static const quint8 MAX_ADDRESS;
    static const quint8 TIMEOUT_DEFAULT;
    static const enum modbus_regs {BROADCAST, READ = 0x03, WRITE_ONE = 0x06} modbus_regs;
    explicit Modbus(QIODevice *device, int m_TimeoutMSecs = TIMEOUT_DEFAULT, QObject *parent = nullptr);
    void setDevice(QIODevice* device, int m_TimeoutMSecs = TIMEOUT_DEFAULT);
    void setTimeout(int);
//    void setEnable(bool);
    bool isEnable();
    void addObserver(ModbusObserverInterface* newObserver);
    void removeObserver(ModbusObserverInterface* newObserver);
    void setDataValue(quint8 addr, quint16 reg, quint16 value);
    void getDataValue(quint8 addr, quint16 reg, quint8 count = 1);

signals:
    void errorOccured(QString);
    void timeoutOccured();

public slots:

private slots:
    void readyRead();
    void bytesWritten(qint64 bytes);

private:
    QPointer<QIODevice> m_Device;
    QQueue<QByteArray*> m_Queue;
    QList<ModbusObserverInterface*> m_vObservers;
    QTimer timeoutTimer;
    int m_TimeoutMSecs;
    QByteArray *m_lastTxPackage;
    int m_bytesWritten;
    quint16 m_lastWriteReg;

//    bool openDevice();
//    void closeDevice();
    quint8 hiBYTE(quint16 value);
    quint8 loBYTE(quint16 value);
    quint16 calcCrc(QByteArray* byteArray);
    void prepareAndWrite(QByteArray* byteArray);
    void rxPacketHandler(QByteArray *rxPacket);
    void makeNotify(quint8 addr, quint16 reg, quint16 value);
    void nothingToSend();
    void tryToSend();
};

#endif // MODBUS_H
