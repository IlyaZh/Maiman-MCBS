#include "SerialThreadWorker.h"
#include <QtSerialPort>
#include "constants.h"

SerialThreadWorker::SerialThreadWorker(/*QObject *parent*/)
    : m_portType{PortType::None}
{

}

SerialThreadWorker::~SerialThreadWorker() {

}

QByteArray SerialThreadWorker::lastPackage() const {
    return m_lastWrittenMsg;
}

// public slots
void SerialThreadWorker::setTimeout(qint64 MSecs) {
    m_timeout = MSecs;
}

void SerialThreadWorker::setDelay(qint64 MSecs) {
    m_delay = MSecs;
}

void SerialThreadWorker::configure(PortType portType, QVariant host, QVariant arg) {
    m_portType = portType;
    m_host = host;
    m_arg = arg;
}

void SerialThreadWorker::writeAndWaitBytes(const QByteArray& msg, qint64 waitBytes, bool priority) {
    QMutexLocker locker(&m_mtx);
    Package pack;
    pack.m_data = msg;
    pack.m_waitSize = waitBytes;
    if(priority)
        m_priorityQueue.enqueue(pack);
    else
        m_queue.enqueue(pack);
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
// TODO: вынеси управление портами в отдельные классы с общим интерфейсом
        while(m_isWork) {
            QThread::msleep(m_delay);
            if(m_sem.tryAcquire(1)) {
                Package package;
                {
                    QMutexLocker locker(&m_mtx);
                    if(!m_priorityQueue.isEmpty())
                        package = m_priorityQueue.dequeue();
                    else
                        package = m_queue.dequeue();

                }
//                qDebug() << package.m_waitSize << package.m_data.toHex(' ');
                m_lastWrittenMsg = package.m_data;
                m_waitRxBytes = package.m_waitSize;
                m_device->write(m_lastWrittenMsg);
                if(!m_device->waitForBytesWritten(Const::NetworkTimeoutMSecs)) {
                    emit timeout();
                } else {
                    QByteArray buffer;
                    bool noError = true;
                    auto avail = m_device->bytesAvailable();
                    if(avail > 0)
                        qDebug() << "available" << avail;
                    while(buffer.size() < package.m_waitSize && noError) {
                        if(!m_device->waitForReadyRead(Const::NetworkTimeoutMSecs)) {
                            if(m_waitRxBytes != 0) {
                                emit timeout();
                                noError = false;
                            }
                        } else {
                            qint64 need = package.m_waitSize-buffer.size();
                            buffer.append(m_device->read(need));
//                            qDebug() << "RX" << buffer.size() << m_waitRxBytes << buffer.toHex(' ');
                        }
                    }
                    if(m_waitRxBytes != 0 && buffer.size() == package.m_waitSize) {
                        qDebug() << "OUT TO" << buffer.size() <<  package.m_waitSize << buffer.toHex(' ');
                        emit readyRead(buffer);
                        buffer.clear();
                    }

                }
//                exec();
            } else {
                emit readyToWrite();
            }
        }
        qDebug() << "[THREAD]" << "Out of while cycle";

        m_device->close();
//        emit finished();
    }
}

