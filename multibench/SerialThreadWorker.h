#pragma once

#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include "constants.h"

class SerialThreadWorker : public QThread
{
    Q_OBJECT
public:
//    enum class PortType {
//        Com,
//        TCP
//    };

    explicit SerialThreadWorker(QObject *parent = nullptr);
    QByteArray lastPackage() const;
public slots:
    void setTimeout(int MSecs);
    void configure(PortType portType, QVariant host, QVariant arg);
//    void write(const QByteArray& msg);
    void writeAndWaitBytes(const QByteArray& msg, qint64 waitBytes, bool priority = false);
    void stop();
//    void setWaitRxBytes(qint64 count);
//    void clear();
private slots:
    void readData();
//    void bytesWritten(qint64 bytes);
signals:
    void connected();
    void readyRead(QByteArray msg);
    void timeout();
    void errorOccured(QString msg);
    void finished();
    void readyToWrite();

private:
    PortType m_portType;
    QVariant m_host;
    QVariant m_arg;
    QPointer<QIODevice> m_device;
    int m_timeout {Const::NetworkTimeoutMSecs};
    qint64 m_waitRxBytes {-1};
    QByteArray m_lastWrittenMsg;
    QByteArray m_buffer;
    QQueue<QPair<QByteArray, qint64>> m_queue;
    QQueue<QPair<QByteArray, qint64>> m_priorityQueue;
    QSemaphore m_sem;
    bool m_isWork {false};
    QMutex m_mtx;

    void run() override;
};

