#include "datasource.h"

DataSource::DataSource(QObject* parent) :
    QObject(parent),
    m_tcpSocket(new QTcpSocket(this)),
    m_serialPort(new QSerialPort(this))
{

}

DataSource::~DataSource() {
    if(m_currentDevice != nullptr) {
        if(m_currentDevice->isOpen()) {
            m_currentDevice->close();
        }
        m_currentDevice->disconnect();
    }
}

void DataSource::setSettings(NetworkType type, QVariant host, QVariant port) {
    m_type = type;
    m_host = host;
    m_port = port;
}

bool DataSource::open() {
    bool state = false;
        if(m_type == NetworkType::SerialPort) {
            m_serialPort->setPortName(m_host.toString());
            m_serialPort->setBaudRate(m_port.toInt());
            m_currentDevice = m_serialPort;
            state = m_serialPort->open(QIODevice::ReadWrite);
        } else if(m_type == NetworkType::Tcp) {
            m_tcpSocket->connectToHost(m_host.toString(), m_port.toInt());
            m_currentDevice = m_tcpSocket;
            state = m_tcpSocket->isOpen();
        }

        connect(m_currentDevice, SIGNAL(bytesWritten(qint64)), this, SIGNAL(bytesWritten(qint64)));
        connect(m_currentDevice, SIGNAL(readyRead()), this, SIGNAL(readyRead()));
        emit deviceOpen(m_currentDevice->isOpen());
    return state;
}


void DataSource::close() {
    if(m_currentDevice != nullptr) {
        m_currentDevice->disconnect();
        m_currentDevice->close();
        emit deviceOpen(m_currentDevice->isOpen());
    }
}

bool DataSource::isOpen() {
    if(m_currentDevice != nullptr) {
        return m_currentDevice->isOpen();
    } else
        return false;
}

qint64 DataSource::write(const QByteArray &byteArray) {
    if(m_currentDevice != nullptr)
        return m_currentDevice->write(byteArray);
    else
        return -1;
}

qint64 DataSource::read(char* data, qint64 maxSize) {
    if(m_currentDevice != nullptr)
        return m_currentDevice->read(data, maxSize);
    else
        return -1;
}

QByteArray DataSource::read(qint64 maxSize) {
    if(m_currentDevice != nullptr)
        return m_currentDevice->read(maxSize);
    else
        return QByteArray();
}

QByteArray DataSource::readAll() {
    if(m_currentDevice != nullptr)
        return m_currentDevice->readAll();
    else
        return QByteArray();
}

QString DataSource::errorString() {
    return m_errorString;
}

