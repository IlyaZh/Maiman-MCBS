#include "SerialThreadWorker.h"
#include <QtSerialPort>

SerialThreadWorker::SerialThreadWorker(QObject *parent)
    : QThread{parent},
      m_portType{PortType::None}
{

}

QByteArray SerialThreadWorker::lastPackage() const {
    return m_lastWrittenMsg;
}

// public slots
void SerialThreadWorker::setTimeout(int MSecs) {
    m_timeout = MSecs;
}

void SerialThreadWorker::configure(PortType portType, QVariant host, QVariant arg) {
    m_portType = portType;
    m_host = host;
    m_arg = arg;
}

void SerialThreadWorker::writeAndWaitBytes(const QByteArray& msg, qint64 waitBytes, bool priority) {
    QMutexLocker locker(&m_mtx);
    auto pair = qMakePair(msg, waitBytes);
    if(priority)
        m_priorityQueue.enqueue(pair);
    else
        m_queue.enqueue(pair);
    m_sem.release(1);
}

void SerialThreadWorker::stop() {
    m_isWork = false;
}

// private slots
void SerialThreadWorker::readData() {
    m_buffer.append(m_device->readAll());
    if(m_buffer.size() > m_waitRxBytes) {
        emit readyRead(m_buffer);
        m_buffer.clear();
    }
}

// private methods

void SerialThreadWorker::run() {
    if(m_host.canConvert(QMetaType::QString) && m_arg.canConvert(QMetaType::Int)) {
        if(m_portType == PortType::Com) {
            auto serialPort = new QSerialPort(m_host.toString(), this);
            serialPort->setBaudRate(m_arg.toInt());
            m_device = serialPort;
        } else if(m_portType == PortType::TCP) {
            auto tcpPort = new QTcpSocket(this);
            tcpPort->bind(QHostAddress(m_host.toString()), m_arg.toInt());
            m_device = tcpPort;
        }

        if(m_device->open(QIODevice::ReadWrite)) {
            emit connected();
            m_isWork = true;
        } else {
            emit errorOccured(m_device->errorString());
            emit finished();
            return;
        }

        while(m_isWork) {
            m_sem.acquire(1);
            QPair<QByteArray, qint64> package;
            {
                QMutexLocker locker(&m_mtx);
                package = m_queue.dequeue();
            }
            m_lastWrittenMsg = package.first;
            m_waitRxBytes = package.second;
            m_device->write(m_lastWrittenMsg);
            if(!m_device->waitForBytesWritten(Const::NetworkTimeoutMSecs)) {
                emit errorOccured(m_device->errorString());
                emit timeout();
            } else {
                if(!m_device->waitForReadyRead(Const::NetworkTimeoutMSecs)) {
                    if(m_waitRxBytes == 0) {
                        emit readyToWrite();
                    } else {
                        emit errorOccured(m_device->errorString());
                        emit timeout();
                    }
                } else {
                    readData();
                }
            }
            exec();
        }

        m_device->close();
        emit finished();
    }
}

