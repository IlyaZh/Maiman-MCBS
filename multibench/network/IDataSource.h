#pragma once

#include <QObject>
#include <QIODevice>

class IDataSource
{
public:
    IDataSource() {}
    virtual ~IDataSource() {}
    virtual void init(const QVariantMap& portSettings) = 0;
    virtual bool open() = 0;
    virtual QIODevice* device() = 0;
};
