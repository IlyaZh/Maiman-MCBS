#ifndef DATASOURCE_MOCK_H
#define DATASOURCE_MOCK_H

#include <QObject>
#include <QMap>
#include <QTimer>
#include "datasource.h"

class DataSource_Mock : public IDataSource
{
public:
    DataSource_Mock(uint deviceCount, int sendDelay);
    void setSettings(NetworkType type, QVariant host, QVariant port) override;
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
    uint deviceCount;
    int sendDelay;
    QTimer timer;
    bool m_isOpen = false;
    QByteArray m_answer;
    QString m_errorString;
    QMap<quint16,QVector<quint16>> m_values;
};

#endif // DATASOURCE_MOCK_H
