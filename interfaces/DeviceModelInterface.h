#ifndef DEVICEINTERFACE_H
#define DEVICEINTERFACE_H

#include <QObject>

class DeviceModelInterface {
public:
    virtual ~DeviceModelInterface();
    virtual quint8 id() = 0;
};

#endif // DEVICEINTERFACE_H
