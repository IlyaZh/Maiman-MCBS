#include "tcpdatasource.h"

#include <QDebug>

TcpDataSource::TcpDataSource() : m_host(QString()), m_port(0) {}

void TcpDataSource::init(const QVariantMap& portSettings) {
  auto host = portSettings["host"];
  auto port = portSettings["port"];
  if (host.canConvert(QMetaType::QString) && port.canConvert(QMetaType::Int)) {
    m_host = host.toString();
    m_port = port.toInt();
  }
}

QIODevice* TcpDataSource::createAndConnect() {
  if (m_device) {
    if (m_device->isOpen()) m_device->close();
    m_device->deleteLater();
  }
  m_device = new QTcpSocket();
  m_device->connectToHost(m_host, m_port);
  qInfo() << "TcpDataSource connect" << m_host << m_port << m_device->isOpen();
  return m_device;
}
