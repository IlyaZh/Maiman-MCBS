#pragma once

#include <QObject>
#include "network/SoftProtocol.h"

class ModbusProtocol : public QObject, public SoftProtocol {
    Q_OBJECT
public:
    enum cmdType {Broadcast, Read = 0x03, WriteOne = 0x06};
    Q_DECLARE_FLAGS(CommandsType, cmdType)
    static quint16 calcCrc(const QByteArray &byteArray);

    explicit ModbusProtocol(QObject* parent = nullptr);
    ~ModbusProtocol() override;
    virtual QByteArray setDataValue(quint8 addr, quint16 reg, quint16 value) override;
    virtual QByteArray getDataValue(quint8 addr, quint16 reg, quint8 count = 1) override;
    virtual DataVector execute(const QByteArray& rxPackage, const QByteArray& lastTxPackage) override;
    virtual bool needWaitForAnswer(const QByteArray& package) override;
private:
    static const quint8 CRC_HTable[256];
    static const quint8 CRC_LTable[256];

    void makeError(const QString& msg);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(ModbusProtocol::CommandsType)