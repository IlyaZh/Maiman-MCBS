#include "DataThread.h"
#include <QtSerialPort>
#include "constants.h"

QByteArray DataThread::lastPackage() const {
    return m_lastWrittenMsg;
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

void DataThread::writeAndWaitBytes(const QByteArray& msg, qint64 waitBytes, bool priority) {
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

void DataThread::stop() {
    m_isWork = false;
}

// private methods

void DataThread::process() //run()
{
    static QSharedPointer<QDeadlineTimer> timeoutTmr;
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
        m_timeout *=2;
        qDebug() << "timeout=" << m_timeout;

        while(m_isWork) {
            QThread::msleep(m_delay);
            if(m_sem.tryAcquire(1)) {
                qDebug() << "ACQUIRED avail=" << m_sem.available();
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
                Q_UNUSED(m_device->readAll());
                m_device->write(m_lastWrittenMsg);
                qDebug() << QDateTime::currentDateTime().time().toString("HH:mm:ss.zzz") << m_lastWrittenMsg.toHex(' ');
                if(!m_device->waitForBytesWritten(Const::NetworkTimeoutMSecs)) {
                    emit timeout();
                } else {
                    QByteArray buffer;
                    timeoutTmr.reset(new QDeadlineTimer(m_timeout));
                    qDebug() << QDateTime::currentDateTime().time().toString("mm:ss.zzz");
                    bool stop = false;
                    while(!timeoutTmr->hasExpired() && !stop) {
//                        if(m_device->bytesAvailable() > 0) {
                            int need = m_waitRxBytes-buffer.size();
                            if(need <= 0) {
                                qDebug() << "rx complete";
                                stop = true;
                            } else {
                                auto rx = m_device->readAll();
                                buffer.append(rx);
                                if(!rx.isEmpty()) {
                                    qDebug() << "rx" << rx.toHex(' ');
                                    qDebug() << QString(m_lastWrittenMsg.at(0)) << "wait =" << m_waitRxBytes << "available" << need;
                                }
                            }
//                        }
                    }
                    if(buffer.isEmpty()) {
                        qDebug() << "timeout" << QDateTime::currentDateTime().time().toString("mm:ss.zzz");
                        emit timeout();
                    } else {
                        qDebug() << "readyRead" << QDateTime::currentDateTime().time().toString("mm:ss.zzz") << buffer.toHex(' ');
                        emit readyRead(buffer);
                    }
                    qDebug() << "====\n";
                }

                if(m_sem.available() == 0)
                    emit readyToWrite();
            }
        }
        m_device->close();
        emit finished();
}

