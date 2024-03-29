#pragma once

#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include "constants.h"
#include "network/IDataSource.h"
#include "appsettings.h"

class DataThread : public QThread
{
    Q_OBJECT
public:
    ~DataThread();

public slots:
    void setTimeout(qint64 MSecs);
    void setDelay(qint64 MSecs);
    void configure(QScopedPointer<IDataSource>& source);
    void writeAndWaitBytes(const QByteArray& msg, qint64 waitBytes, bool isShortTimeout);
    void stop();

Q_SIGNALS:
    void connected();
    void readyRead(const QByteArray msg, const QByteArray lastPackage);
    void timeout(const QByteArray lastPackage);
    void errorOccured(const QString msg);
    void readyToWrite();

private:
    struct Package {
        QByteArray m_data {};
        qint64 m_waitSize {0};
        bool isShortTimeout = false;
    };

    qint64 m_delay {Const::kNetworkDelayMSecs::defaultValue};
    qint64 m_timeout {Const::kNetworkTimeoutMSecs::defaultValue};
    qint64 m_waitRxBytes {-1};
    QByteArray m_lastWrittenMsg;
    Package m_next;
    bool m_isWork {true};
    QMutex m_mtx;
    QScopedPointer<IDataSource> m_dataSource;
//    QWaitCondition m_condition;
    QSemaphore m_sem;
    bool m_firstRun {true};

    void run() override;
};

