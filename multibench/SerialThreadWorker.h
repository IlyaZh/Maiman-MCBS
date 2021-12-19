#pragma once

#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include "constants.h"
#include "network/IDataSource.h"

class SerialThreadWorker : public QThread
{
    Q_OBJECT
public:
    explicit SerialThreadWorker(/*QObject *parent = nullptr*/);
    ~SerialThreadWorker() = default;
    QByteArray lastPackage() const;
public slots:
    void setTimeout(qint64 MSecs);
    void setDelay(qint64 MSecs);
    void configure(IDataSource* source);
    void writeAndWaitBytes(const QByteArray& msg, qint64 waitBytes, bool priority = false);
    void stop();
private slots:
//    void readData();
//    void bytesWritten(qint64 bytes);
signals:
    void connected();
    void readyRead(QByteArray msg);
    void timeout();
    void errorOccured(QString msg);
//    void finished();
    void readyToWrite();

private:
    struct Package {
        QByteArray m_data {};
        qint64 m_waitSize {0};
    };

//    PortType m_portType;
//    QVariant m_host;
//    QVariant m_arg;
//    QScopedPointer<QIODevice> m_device;
    qint64 m_timeout {Const::NetworkTimeoutMSecs};
    qint64 m_delay {Const::NetworkDelayMSecs};
    qint64 m_waitRxBytes {-1};
    QByteArray m_lastWrittenMsg;
//    QByteArray m_buffer;
    QQueue<Package> m_queue;
    QQueue<Package> m_priorityQueue;
    QSemaphore m_sem;
    bool m_isWork {false};
    QMutex m_mtx;
    QScopedPointer<IDataSource> m_dataSource;

    void run() override;
};

