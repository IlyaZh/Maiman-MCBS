#pragma once

#include "IDataSource.h"
#include <QtNetwork>

class TcpDataSource : public IDataSource
{
    QPointer<QTcpSocket> m_device;
    QString m_host {};
    int m_port {0};
public:
    TcpDataSource();
    ~TcpDataSource() override = default;
    void init(const QVariantMap& portSettings) override;
    QIODevice* createAndConnect() override;
};



