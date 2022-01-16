#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QtCore>
#include <QtNetwork>
#include <QtSerialPort>

enum class NetworkType { None, Tcp, SerialPort };

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
