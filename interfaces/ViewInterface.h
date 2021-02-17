#ifndef VIEWINTERFACE_H
#define VIEWINTERFACE_H

#include <QObject>
#include <QMap>
#include "DevCommand.h"

class ViewInterface {
public:
    ViewInterface();
    virtual ~ViewInterface() = 0;

    virtual void clear() = 0;
    virtual void addDevice(quint8 addr, quint16 id) = 0;
    virtual void removeDevice(quint8 addr) = 0;
    virtual bool hasDevice(quint8 addr) = 0;
    virtual quint16 deviceId(quint8 addr) = 0;
    virtual void setDeviceData(quint8 addr, DevCommand* cmd) = 0;
    virtual void setDeviceLink(quint8 addr, bool link);
};

#endif // VIEWINTERFACE_H
