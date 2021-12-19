#include "serialdatasource.h"

SerialDataSource::SerialDataSource() :
    m_device(new QSerialPort())
{

}

void SerialDataSource::init(const QVariantMap& portSettings) {
    auto port = portSettings["comport"];
    auto baud = portSettings["baudrate"];
    if(port.canConvert(QMetaType::QString) && baud.canConvert(QMetaType::Int)) {
        m_port = port.toString();
        m_baud = baud.toInt();
    }
}

QIODevice* SerialDataSource::createAndConnect() {
    m_device = new QSerialPort;
    m_device->setBaudRate(m_baud);
    m_device->setPortName(m_port);
    m_device->open(QIODevice::ReadWrite);
    return m_device;
}

//bool SerialDataSource::open() {
//    m_device->setBaudRate(m_baud);
//    m_device->setPortName(m_port);
//    return m_device->open(QIODevice::ReadWrite);
//}

bool SerialDataSource::isOpen() {
    return (m_device->isOpen());
}

//QIODevice* SerialDataSource::device() {
//    return m_device.get();
//}
