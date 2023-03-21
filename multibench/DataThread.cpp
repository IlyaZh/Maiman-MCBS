#include "DataThread.h"
#include <QtSerialPort>
#include "constants.h"

enum {portTimeoutCycle = 5};

DataThread::~DataThread() {
    m_mtx.lock();
    m_isWork = false;
    if(m_sem.available() == 0)
        m_sem.release();
    m_mtx.unlock();
    wait();
}

// public slots
void DataThread::setTimeout(qint64 MSecs) {
    m_timeout = MSecs;
}

void DataThread::setDelay(qint64 MSecs) {
    m_delay = MSecs;
}

void DataThread::configure(QScopedPointer<IDataSource>& source) {
    m_dataSource.swap(source);
}

void DataThread::writeAndWaitBytes(const QByteArray& msg, qint64 waitBytes, bool isDisconnectedDevice) {
    QMutexLocker locker(&m_mtx);
    Package pack;
    pack.m_data = msg;
    pack.m_waitSize = waitBytes;
    pack.isDisconnected = isDisconnectedDevice;
    m_next = pack;

    if(!isRunning()) {
        start(LowPriority);
    } else {
        m_sem.release();
    }
}

void DataThread::stop() {
    m_mtx.lock();
    m_isWork = false;
    if(isRunning()){
        if(m_sem.available() == 0)
            m_sem.release();
    }
    m_mtx.unlock();
}

// private methods

void DataThread::run() {
    int waitForConnected = portTimeoutCycle;
    QScopedPointer<QIODevice> m_device(m_dataSource->createAndConnect());
    while(!m_device->isOpen()) {
        if(waitForConnected == 0) {
            emit errorOccured("Can't сonnect. " + m_device->errorString());
            return;
        }
        --waitForConnected;
        QThread::msleep(100);
    }
    emit connected();

    while(m_isWork) {
        Package package;
        {
            QMutexLocker locker(&m_mtx);
            package = m_next;

        }
        qint64 networkTimeout = m_timeout;
        m_lastWrittenMsg = package.m_data;
        m_waitRxBytes = package.m_waitSize;
        if(package.isDisconnected){
            networkTimeout = Const::NetworkDisconnectedTimeout;
        }
        m_device->write(m_lastWrittenMsg);
//        qDebug() << QDateTime::currentDateTime().time().toString("HH:mm:ss.zzz") << m_lastWrittenMsg.toHex(' ');
        if(!m_device->waitForBytesWritten(networkTimeout)) {
            qDebug() << "NOT WRITTEN";
            emit timeout(m_lastWrittenMsg);
        } else {
            QByteArray buffer;
                QThread::msleep(10);
                while(m_device->waitForReadyRead(networkTimeout))
                {
                    int need = m_waitRxBytes-buffer.size();
                    if(need > 0){
                        auto rx = m_device->readAll();
                        buffer.append(rx);
                    }
                    if(m_waitRxBytes-buffer.size() <= 0) {
                        break;
                    }
                }
            if(buffer.isEmpty()) {
                qDebug() << "timeout" << QDateTime::currentDateTime().time().toString("mm:ss.zzz");
                emit timeout(m_lastWrittenMsg);
            } else {
                emit readyRead(buffer, m_lastWrittenMsg);
            }
        }
        if(!m_isWork) break;
        emit readyToWrite();
        m_sem.acquire();
        QThread::msleep(m_delay);
    }
    m_device->close();
    qDebug()<< "Thread run quit";
}
