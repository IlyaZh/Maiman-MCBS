#pragma once

#include <QtNetwork>
#include <QtSerialPort>

#include "IDataSource.h"

class SerialDataSource : public IDataSource {
  QPointer<QSerialPort> m_device;
  QString m_port{};
  int m_baud{0};

 public:
  SerialDataSource();
  ~SerialDataSource() override = default;
  void init(const QVariantMap& portSettings) override;
  QIODevice* createAndConnect() override;
};
