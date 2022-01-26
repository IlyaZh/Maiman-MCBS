#include "DataThread.h"
#include <QtSerialPort>
#include "constants.h"

DataThread::~DataThread() {
    qDebug()<<"Distructor"<<m_isWork;
    m_mtx.lock();
    m_isWork = false;
//    m_condition.wakeOne();
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

void DataThread::writeAndWaitBytes(const QByteArray& msg, qint64 waitBytes) {
    QMutexLocker locker(&m_mtx);
    Package pack;
    pack.m_data = msg;
    pack.m_waitSize = waitBytes;
    m_next = pack;

    if(!isRunning()) {
        qDebug()<<"DataThread::writeAndWaitBytes is not running";
        start(LowPriority);
    } else {
        qDebug()<<"DataThread::writeAndWaitBytes is running";
//        m_condition.wakeOne();
        m_sem.release();
    }
}

void DataThread::stop() {
    qDebug()<<"DataThread:stop"<<m_isWork;
    m_mtx.lock();
    m_isWork = false;
    if(isRunning()){
//        m_condition.wakeOne();
        if(m_sem.available() == 0)
            m_sem.release();
        qDebug()<<"DataThread:stop is running"<<m_isWork;
    }
    m_mtx.unlock();
}

// private methods

void DataThread::run() {
    int waitForConnected = m_timeout;
    bool out {false};
    QScopedPointer<QIODevice> m_device(m_dataSource->createAndConnect());
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
        qDebug()<<"DataThread::run m_isWork"<<m_isWork;
        Package package;
        {
            QMutexLocker locker(&m_mtx);
            package = m_next;

        }
        m_lastWrittenMsg = package.m_data;
        m_waitRxBytes = package.m_waitSize;

        m_device->write(m_lastWrittenMsg);
        qDebug() << QDateTime::currentDateTime().time().toString("HH:mm:ss.zzz") << m_lastWrittenMsg.toHex(' ');
        if(!m_device->waitForBytesWritten(m_timeout)) {
            qDebug() << "NOT WRITTEN";
            emit timeout(m_lastWrittenMsg);
        } else {
            QByteArray buffer;
                QThread::msleep(10);
                qDebug() << "bytes = " << m_device->bytesAvailable();
                while(m_device->waitForReadyRead(m_timeout))
                {
                    int need = m_waitRxBytes-buffer.size();
                    if(need > 0){
                        auto rx = m_device->readAll();
                        buffer.append(rx);
                        if(!rx.isEmpty()) {
                            qDebug() << "rx" << rx.toHex(' ');
                            qDebug() << QString(m_lastWrittenMsg.at(0)) << "wait =" << m_waitRxBytes << "available" << need;
                        }
                    }
                    if(m_waitRxBytes-buffer.size() <= 0) {
                        qDebug() << "rx complete";
                        break;
                    }
                }
            if(buffer.isEmpty()) {
                qDebug() << "timeout" << QDateTime::currentDateTime().time().toString("mm:ss.zzz");
                emit timeout(m_lastWrittenMsg);
            } else {
                qDebug() << "readyRead" << QDateTime::currentDateTime().time().toString("mm:ss.zzz") << buffer.toHex(' ');
                emit readyRead(buffer, m_lastWrittenMsg);
            }
            qDebug() << "====\n";
        }
        if(!m_isWork) break;
        emit readyToWrite();
//        qDebug()<<__FILE__<<__LINE__<<"DataThread::run before lock";
//        m_mtx.lock();
        qDebug()<<__FILE__<<__LINE__<<"DataThread::run before wait condition";
        if(out) {
            qDebug() << "OUT IS TRUE";
            break;
        }
        m_sem.acquire();
//        if(m_condition.wait(&m_mtx, 5000))
//            qDebug() << "waitCondition true";
//        else {
//            qDebug() << "waitCondition false" << m_isWork;
//            out = true;
//        }
//        m_mtx.unlock();
        qDebug() << __FILE__ << __LINE__ << "DataThread::run before sleep" << m_isWork <<m_lastWrittenMsg;
        QThread::msleep(m_delay);
    }
    m_device->close();
    qDebug()<< "Thread run quit";
}
