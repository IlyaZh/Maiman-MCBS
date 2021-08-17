#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QSerialPort>
#include <QTcpSocket>
#include <QIODevice>

enum class NetworkType { NONE, Tcp, SerialPort };

class IDataSource : public QObject {
    Q_OBJECT
public:
    IDataSource(QObject *parent = nullptr) : QObject(parent) {}
    virtual void setSettings(NetworkType type, QVariant host, QVariant port) = 0;
//    virtual QIODevice* device() = 0;
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool isOpen() = 0;
//    virtual qint64 write(const char* data, qint64 maxSize) = 0;
//    virtual qint64 write(const char* data) = 0;
    virtual qint64 write(const QByteArray &byteArray) = 0;
    virtual qint64 read(char* data, qint64 maxSize) = 0;
    virtual QByteArray read(qint64 maxSize) = 0;
    virtual QByteArray readAll() = 0;
    virtual QString errorString() = 0;

signals:
    void deviceOpen(bool isOpen);
    void errorOccured(QString msg);
    void bytesWritten(qint64 bytes);
    void readyRead();
};

class DataSource : public IDataSource
{

public:
    explicit DataSource(QObject* parent = nullptr);
    ~DataSource();
//    static DataSource* create();
    void setSettings(NetworkType type, QVariant host, QVariant port) override;
//    QIODevice* device() override;
    bool open() override;
    void close() override;
    bool isOpen() override;
//    qint64 write(const char* data, qint64 maxSize) override;
//    qint64 write(const char* data) override;
    qint64 write(const QByteArray &byteArray) override;
    qint64 read(char* data, qint64 maxSize) override;
    QByteArray read(qint64 maxSize) override;
    QByteArray readAll() override;
    QString errorString() override;

private:
//    static DataSource* m_im;
    QString m_errorString;

    QTcpSocket* m_tcpSocket;
    QSerialPort* m_serialPort;
    QIODevice* m_currentDevice;
    NetworkType m_type = NetworkType::NONE;
    QVariant m_host;
    QVariant m_port;

};

#endif // DATASOURCE_H
