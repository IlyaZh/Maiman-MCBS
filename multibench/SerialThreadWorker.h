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
    explicit SerialThreadWorker();
    ~SerialThreadWorker() = default;
    QByteArray lastPackage() const;

public slots:
    void setTimeout(qint64 MSecs);
    void setDelay(qint64 MSecs);
    void configure(IDataSource* source);
    void writeAndWaitBytes(const QByteArray& msg, qint64 waitBytes, bool priority = false);
    void stop();

signals:
    void connected();
    void readyRead(QByteArray msg);
    void timeout();
    void errorOccured(QString msg);
    void readyToWrite();

private:
    struct Package {
        QByteArray m_data {};
        qint64 m_waitSize {0};
    };

    qint64 m_timeout {Const::NetworkTimeoutMSecs};
    qint64 m_delay {Const::NetworkDelayMSecs};
    qint64 m_waitRxBytes {-1};
    QByteArray m_lastWrittenMsg;
    QQueue<Package> m_queue;
    QQueue<Package> m_priorityQueue;
    QSemaphore m_sem;
    bool m_isWork {true};
    QMutex m_mtx;
    QScopedPointer<IDataSource> m_dataSource;

    void run() override;
};

