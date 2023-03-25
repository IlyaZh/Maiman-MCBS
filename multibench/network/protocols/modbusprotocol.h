#pragma once

#include <QObject>

#include "network/SoftProtocol.h"

class ModbusProtocol : public QObject, public SoftProtocol {
  Q_OBJECT
 public:
  enum cmdType { Broadcast, Read = 0x03, WriteOne = 0x06 };
  Q_DECLARE_FLAGS(CommandsType, cmdType)
  static quint16 calcCrc(const QByteArray& byteArray);

  explicit ModbusProtocol(QObject* parent = nullptr);
  QByteArray setDataValue(quint8 addr, quint16 reg, quint16 value) override;
  QByteArray getDataValue(quint8 addr, quint16 reg, quint8 count = 1) override;
  DataVector execute(const QByteArray& rxPackage,
                     const QByteArray& lastTxPackage) override;
  bool needWaitForAnswer(const QByteArray& package) override;
  qint64 waitForBytes(const QByteArray& package) override;

 private:
  static const QVector<quint8> CRC_HTable;
  static const QVector<quint8> CRC_LTable;

  void makeError(QStringView msg);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(ModbusProtocol::CommandsType)
