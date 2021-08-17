#ifndef MODBUS_H
#define MODBUS_H

#include <QObject>


#include "network/SoftProtocol.h"

class ModbusProtocol : public QObject, public SoftProtocol {
    Q_OBJECT
public :
    static const int maxQueueSize = 100;
    enum cmdType {Broadcast, Read = 0x03, WriteOne = 0x06};
    Q_DECLARE_FLAGS(CommandsType, cmdType)
    explicit ModbusProtocol(IDataSource* device = nullptr, int timeout_ms = SoftProtocol::TimeoutDefault, int delay_ms = SoftProtocol::DelayDefault, QObject* parent = nullptr);
    ~ModbusProtocol();
    void setDataValue(quint8 addr, quint16 reg, quint16 value) override;
    void getDataValue(quint8 addr, quint16 reg, quint8 count = 1) override;
    static quint16 calcCrc(const QByteArray &byteArray);
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
    void rxPacketHandler(const QByteArray &rxPacket) override;
    void prepareAndWrite(QByteArray &byteArray) override;
private slots:
    void readyRead_Slot() override;
    void bytesWritten_Slot(qint64 bytes) override;
    void timeout_Slot();
    void delayBeforeSend_Slot() override;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(ModbusProtocol::CommandsType)


#endif // MODBUS_H
