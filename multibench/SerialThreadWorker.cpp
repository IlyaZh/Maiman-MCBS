#include "SerialThreadWorker.h"
#include <QtSerialPort>
#include "constants.h"

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

void SerialThreadWorker::configure(QScopedPointer<IDataSource>& source) {
    m_dataSource.swap(source);
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

// private methods

void SerialThreadWorker::run() {
    QScopedPointer<QIODevice> m_device(m_dataSource->createAndConnect());
        int waitForConnected = Const::NetworkTimeoutMSecs;
        while(!m_device->isOpen()) {
            --waitForConnected;
            if(waitForConnected == 0) {
                emit errorOccured("Can't сonnect. " + m_device->errorString());
                return;
            }
            QThread::msleep(1);
        }
        emit connected();

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
                m_lastWrittenMsg = package.m_data;
                m_waitRxBytes = package.m_waitSize;
                m_device->write(m_lastWrittenMsg);
                if(!m_device->waitForBytesWritten(Const::NetworkTimeoutMSecs)) {
                    emit timeout();
                } else {
                    QByteArray buffer;
                    bool noError = true;
                    while(buffer.size() < package.m_waitSize && noError) {
                        if(!m_device->waitForReadyRead(Const::NetworkTimeoutMSecs)) {
                            if(m_waitRxBytes != 0) {
                                emit timeout();
                                noError = false;
                            }
                        } else {
                            qint64 need = package.m_waitSize-buffer.size();
                            buffer.append(m_device->read(need));
                        }
                    }
                    if(m_waitRxBytes != 0 && buffer.size() == package.m_waitSize) {
                        emit readyRead(buffer);
                        buffer.clear();
                    }

                }

                if(m_sem.available() == 0)
                    emit readyToWrite();
            }
        }
        m_device->close();

}

