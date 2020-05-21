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
    static const quint8 MODBUS_MAX_ADDRESS_CONST;
    static const quint8 MODBUS_TIMEOUT_DEFAULT;
    static const enum modbus_regs {READ = 0x03, WRITE_ONE = 0x06} modbus_regs;
    explicit Modbus(QIODevice *device, QString deviceName, int m_TimeoutMSecs = MODBUS_TIMEOUT_DEFAULT, QObject *parent = nullptr);
    ~Modbus();
    void setDevice(QIODevice* device, QString deviceName, int m_TimeoutMSecs = MODBUS_TIMEOUT_DEFAULT);
    void setTimeout(int);
    void setEnable(bool);
    bool isEnable();
    void addObserver(ModbusObserverInterface& newObserver);
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
    QString m_DeviceName;
    QQueue<QByteArray*> m_Queue;
    QVector<ModbusObserverInterface*> m_ObserverVector;
    QTimer timeoutTimer;
    int m_TimeoutMSecs;
    QByteArray *m_lastTxPackage;
    int m_bytesWritten;
    quint16 m_lastWriteReg;

    bool openDevice();
    void closeDevice();
    quint8 hiBYTE(quint16 value);
    quint8 loBYTE(quint16 value);
    quint16 calcCrc(QByteArray* byteArray);
    void prepareToWrite(QByteArray* byteArray);
    void rxPacketHandler(QByteArray *rxPacket);
    void makeNotify(quint8 addr, quint16 reg, quint16 value);
};

#endif // MODBUS_H
