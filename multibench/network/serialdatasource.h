#pragma once

#include "IDataSource.h"
#include <QtNetwork>
#include <QtSerialPort>

class SerialDataSource : public IDataSource
{
    QPointer<QSerialPort> m_device;
    QString m_port {};
    int m_baud {0};
public:
    SerialDataSource();
    ~SerialDataSource() override = default;
    void init(const QVariantMap& portSettings) override;
    QIODevice* createAndConnect() override;
};



