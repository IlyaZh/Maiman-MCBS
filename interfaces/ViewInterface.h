#ifndef VIEWINTERFACE_H
#define VIEWINTERFACE_H

#include <QObject>
#include <QMap>
#include "device/devicemodel.h"
#include "constants.h"
#include "device/device.h"

class ViewInterface {
public:
    ViewInterface() { m_devices.resize(Const::MaxDevices); m_devices.fill(nullptr); }
//    virtual ~ViewInterface() = 0;

    virtual void createdDevice( Device* pDev) = 0;
    virtual void removeDevice( Device* pDev) = 0;
    virtual void removeAllDevices() = 0;

    /*virtual void clearDevices() = 0;
    virtual void addDevice(quint8 addr, const Device* dev) = 0;
    virtual void removeDevice(quint8 addr) = 0;
    virtual bool hasDevice(quint8 addr) = 0;
    virtual quint16 deviceId(quint8 addr) = 0;
    virtual void setDeviceData(quint8 addr, quint16 cmd, quint16 value) = 0;*/
//    virtual void setDeviceLink(quint8 addr, bool link) = 0;

protected:
    QVector<DeviceModel*> m_devices;
};

#endif // VIEWINTERFACE_H
