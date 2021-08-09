#ifndef MODBUS_H
#define MODBUS_H

#include <QObject>


#include "network/SoftProtocol.h"

class ModbusProtocol : public QObject, public SoftProtocol {
    Q_OBJECT
public :
    enum cmdType {Broadcast, Read = 0x03, WriteOne = 0x06};
    Q_DECLARE_FLAGS(CommandsType, cmdType)
    explicit ModbusProtocol(IDataSource* device = nullptr, int timeout_ms = SoftProtocol::TimeoutDefault, int delay_ms = SoftProtocol::DelayDefault, QObject* parent = nullptr);
    ~ModbusProtocol();
    void setDataValue(quint8 addr, quint16 reg, quint16 value) override;
    void getDataValue(quint8 addr, quint16 reg, quint8 count = 1) override;
private:
    static const quint8 CRC_HTable[256];
    static const quint8 CRC_LTable[256];
    QTimer m_timeoutTmr;
    int m_timeoutMs;
    QByteArray m_lastTxPackage;
    quint16 m_lastWriteReg;
    QQueue<QByteArray> m_Queue;
    QQueue<QByteArray> m_PriorityQueue;
    // private methods
    quint16 calcCrc(const QByteArray &byteArray) override;
    void rxPacketHandler(const QByteArray &rxPacket) override;
    void prepareAndWrite(QByteArray &byteArray) override;
private slots:
    void readyRead_Slot() override;
    void bytesWritten_Slot(qint64 bytes) override;
    void timeout_Slot();
    void delayBeforeSend_Slot() override;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(ModbusProtocol::CommandsType)

//class ModbusOld : public SoftProtocolOld
//{
//    Q_OBJECT
//public:
//    static const enum modbus_regs {BROADCAST, READ = 0x03, WRITE_ONE = 0x06} modbus_regs;
//    explicit ModbusOld(QString name, int timeoutMSecs = SoftProtocolOld::TIMEOUT_DEFAULT, int delayMSecs = SoftProtocolOld::DELAY_DEFAULT, QObject *parent = nullptr);
//    ~ModbusOld();
//    void stop() override;
//    void setDataValue(quint8 addr, quint16 reg, quint16 value) override;
//    void getDataValue(quint8 addr, quint16 reg, quint8 count = 1) override;

//public slots:

//private slots:
//    void readyRead() override;
//    void bytesWritten(qint64 bytes) override;
//    void timeout() override;
//    void delayBeforeSendCallback() override;

//private:
//    static const quint8 CRC_HTable[256];
//    static const quint8 CRC_LTable[256];

//    quint16 m_lastWriteReg;
//    QQueue<QByteArray> m_PriorityQueue;
////    bool openDevice();
////    void closeDevice();
//    quint16 calcCrc(QByteArray &byteArray) override;
//    void prepareAndWrite(QByteArray &byteArray) override;
//    void rxPacketHandler(QByteArray &rxPacket) override;
//    void tryToSend() override;
//};

#endif // MODBUS_H
