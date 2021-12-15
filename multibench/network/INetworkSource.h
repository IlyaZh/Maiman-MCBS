#pragma once

#include <QtCore>
#include <QtNetwork>

class INetworkSource {
    QSharedPointer<QIODevice> m_device;
public:
    INetworkSource() {}
    virtual ~INetworkSource() = 0;

    virtual void setConfig(QVariant host, QVariant arg) = 0;
    virtual QIODevice* device() const {
        return m_device.get();
    }
    virtual bool open() = 0;
    virtual void close() {
        if(m_device && m_device->isOpen())
            m_device->close();
    }
};
