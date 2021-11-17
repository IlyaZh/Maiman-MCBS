#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QtCore>
#include <QtNetwork>
#include <QtSerialPort>
//#include <QObject>
//#include <QSerialPort>
//#include <QTcpSocket>
//#include <QIODevice>
//#include <QPointer>
//#include <QScopedPointer>

enum class NetworkType { None, Tcp, SerialPort };

//class DataSource : public QObject {
//    Q_OBJECT
//public:
//    DataSource(QObject *parent = nullptr) : QObject(parent) {}
//    virtual void setSettings(NetworkType type, QVariant host, QVariant port) = 0;
//    virtual bool open() = 0;
//    virtual void close() = 0;
//    virtual bool isOpen() = 0;
//    virtual qint64 write(const QByteArray &byteArray) = 0;
//    virtual qint64 read(char* data, qint64 maxSize) = 0;
//    virtual QByteArray read(qint64 maxSize) = 0;
//    virtual QByteArray readAll() = 0;
//    virtual QString errorString() = 0;


//};

class DataSource : public QObject //public DataSource
{
Q_OBJECT
public:
    explicit DataSource(QObject* parent = nullptr);
    ~DataSource();
    void setSettings(NetworkType type, QVariant host, QVariant port);
    bool open();
    void close();
    bool isOpen();
    qint64 write(const QByteArray &byteArray);
    qint64 read(char* data, qint64 maxSize);
    QByteArray read(qint64 maxSize);
    QByteArray readAll();
    QString errorString();

signals:
    void deviceOpen(bool isOpen);
    void errorOccured(QString msg);
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QString m_errorString;

    QTcpSocket* m_tcpSocket {nullptr};
    QSerialPort* m_serialPort {nullptr};
    QIODevice* m_currentDevice {nullptr};
    NetworkType m_type = NetworkType::None;
    QVariant m_host;
    QVariant m_port;

};

#endif // DATASOURCE_H
