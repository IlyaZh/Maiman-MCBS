#pragma once

#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include "constants.h"
#include "network/IDataSource.h"

class DataThread : public
        QThread
//        QObject
{
    Q_OBJECT
public:
    ~DataThread();

public slots:
    void setTimeout(qint64 MSecs);
    void setDelay(qint64 MSecs);
    void configure(QScopedPointer<IDataSource>& source);
    void writeAndWaitBytes(const QByteArray& msg, qint64 waitBytes, bool priority = false);
    void stop();
//    void process();

Q_SIGNALS:
    void connected();
    void readyRead(const QByteArray msg, const QByteArray lastPackage);
    void timeout(const QByteArray lastPackage);
    void errorOccured(const QString msg);
    void readyToWrite();
//    void finished();

private:
    struct Package {
        QByteArray m_data {};
        qint64 m_waitSize {0};
    };

    qint64 m_timeout {Const::NetworkTimeoutMSecs::defaultValue};
    qint64 m_delay {Const::NetworkDelayMSecs};
    qint64 m_waitRxBytes {-1};
    QByteArray m_lastWrittenMsg;
    Package m_next;
//    QQueue<Package> m_queue;
//    QQueue<Package> m_priorityQueue;
//    QSemaphore m_sem;
    bool m_isWork {true};
    QMutex m_mtx;
    QScopedPointer<IDataSource> m_dataSource;
//    QQueue<QByteArray> outQueue;
    QWaitCondition m_condition;
    bool m_firstRun {true};

    void run() override;
    void makeTimeout(const QByteArray& lastPackage);
    void makeAnswer(const QByteArray& msg, const QByteArray& lastPackage);
};

