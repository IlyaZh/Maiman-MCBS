#include "serialdatasource.h"

SerialDataSource::SerialDataSource() : m_device(new QSerialPort()) {}

void SerialDataSource::init(const QVariantMap& portSettings) {
  auto port = portSettings["comport"];
  auto baud = portSettings["baudrate"];
  if (port.canConvert(QMetaType::QString) && baud.canConvert(QMetaType::Int)) {
    m_port = port.toString();
    m_baud = baud.toInt();
  }
}

QIODevice* SerialDataSource::createAndConnect() {
  if (m_device) {
    if (m_device->isOpen()) m_device->close();
    m_device->deleteLater();
  }
  m_device = new QSerialPort;
  m_device->setBaudRate(m_baud);
  m_device->setPortName(m_port);
  m_device->open(QIODevice::ReadWrite);
  return m_device;
}
