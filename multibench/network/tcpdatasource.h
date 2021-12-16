#pragma once

#include "IDataSource.h"
#include <QtNetwork>

class TcpDataSource : public IDataSource
{
    QScopedPointer<QTcpSocket> m_device;
    QString m_host;
    int m_port;
public:
    TcpDataSource();
    ~TcpDataSource() override = default;
    void init(const QVariantMap& portSettings) override;
    bool open() override;
    QIODevice* device() override;
};

