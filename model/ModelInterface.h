#ifndef MODELINTERFACE_H
#define MODELINTERFACE_H

#include <QObject>
#include <QIODevice>
#include "model/ModelCommands.h"
#include "enums.h"

class ModelInterface {
public:
//    virtual ~ModelInterface() = 0;
    virtual void start(CONNECT_PROTOCOL protocol, QString host, int port) = 0;
    virtual bool isStart() { return false; }
    virtual void stop() = 0;
    virtual void setDeviceCommand(quint8 addr, quint16 command, quint16 value) = 0;
    virtual void rescanNetwork() = 0;
};

#endif // MODELINTERFACE_H
