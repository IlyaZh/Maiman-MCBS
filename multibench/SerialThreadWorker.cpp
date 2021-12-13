#include "SerialThreadWorker.h"
#include <QtSerialPort>

SerialThreadWorker::SerialThreadWorker(/*QObject *parent*/)
    : m_portType{PortType::None}
{

}

SerialThreadWorker::~SerialThreadWorker() {
    qDebug() << "~SerialThreadWorker()";
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
/*void SerialThreadWorker::readData() {
    m_buffer.append(m_device->readAll());
    if(m_buffer.size() > m_waitRxBytes) {
        emit readyRead(m_buffer);
        m_buffer.clear();
    }
}*/

// private methods

void SerialThreadWorker::run() {
    qDebug() << "Thread start" << m_host.canConvert(QMetaType::QString) << m_arg.canConvert(QMetaType::Int);
    if(m_host.canConvert(QMetaType::QString) && m_arg.canConvert(QMetaType::Int)) {
        QScopedPointer<QIODevice> m_device;
        if(m_portType == PortType::Com) {
            auto serialPort = new QSerialPort(m_host.toString());
            serialPort->setBaudRate(m_arg.toInt());
            m_device.reset(serialPort);
            if(serialPort->open(QIODevice::ReadWrite)) {
                emit connected();
                m_isWork = true;
                qDebug() << "PORT IS OPEN";
            } else {
                emit errorOccured(serialPort->errorString());
                qDebug() << "PORT ERROR " << serialPort->errorString();
                return;
            }
        } else if(m_portType == PortType::TCP) {
            auto tcpPort = new QTcpSocket();
            qDebug() << "host" << m_host.toString() << m_arg.toInt();
            m_device.reset(tcpPort);
            tcpPort->connectToHost(QHostAddress(m_host.toString()), m_arg.toInt());
            if(tcpPort->waitForConnected()) {
                emit connected();
                m_isWork = true;
                qDebug() << "PORT IS OPEN";
            } else {
                qDebug() << "wait for connected" << tcpPort->errorString();
                emit errorOccured(tcpPort->errorString());
                return;
            }
        }



        while(m_isWork) {
            if(m_sem.tryAcquire(1)) {
                QPair<QByteArray, qint64> package;
                {
                    QMutexLocker locker(&m_mtx);
                    if(!m_priorityQueue.isEmpty())
                        package = m_priorityQueue.dequeue();
                    else
                        package = m_queue.dequeue();

                }
                qDebug() << package.second << package.first.toHex(' ');
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
                        m_buffer.append(m_device->readAll());
                        if(m_buffer.size() > m_waitRxBytes) {
                            emit readyRead(m_buffer);
                            m_buffer.clear();
                        }
                    }
                }
//                exec();
            }
        }
        qDebug() << "[THREAD]" << "Out of while cycle";

        m_device->close();
//        emit finished();
    }
}

