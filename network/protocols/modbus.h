#ifndef MODBUS_H
#define MODBUS_H

#include <QObject>


#include "network/SoftProtocol.h"

class Modbus : public SoftProtocol
{
public:
    static const enum modbus_regs {BROADCAST, READ = 0x03, WRITE_ONE = 0x06} modbus_regs;
    explicit Modbus(QString name, QIODevice *device = nullptr, int m_TimeoutMSecs = SoftProtocol::TIMEOUT_DEFAULT, QObject *parent = nullptr);
    ~Modbus();
    void setDataValue(quint8 addr, quint16 reg, quint16 value) override;
    void getDataValue(quint8 addr, quint16 reg, quint8 count = 1) override;

public slots:

private slots:
    void readyRead() override;
    void bytesWritten(qint64 bytes) override;

private:
    quint16 m_lastWriteReg;

//    bool openDevice();
//    void closeDevice();
    quint16 calcCrc(QByteArray* byteArray) override;
    void prepareAndWrite(QByteArray* byteArray) override;
    void rxPacketHandler(QByteArray *rxPacket) override;
    void tryToSend() override;
};

#endif // MODBUS_H
