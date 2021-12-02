#pragma once

#include <QObject>
//#include "../device/commandsettings.h"
#include "device.h"
#include "constants.h"
#include <QPixmap>
#include <QMap>

class Device;
class DevCommand;
struct CommandSettings;

struct DeviceDelays {
    static const quint16 COM_COMMAND_MIN_SEND_DELAY = 50;
    static const quint16 COM_COMMAND_MAX_SEND_DELAY = 1000;
    static const quint16 COM_STOP_DELAY_MS = 300;

    DeviceDelays(quint16 stopDelayMs = COM_STOP_DELAY_MS,
                 quint16 minCommandDelayMs = COM_COMMAND_MIN_SEND_DELAY,
                 quint16 maxCommandDelayMs = COM_COMMAND_MAX_SEND_DELAY,
                 quint16 timeout_ms = Const::ComPortTimeout)
        : stopDelayMs(stopDelayMs),
          minCommandDelayMs(minCommandDelayMs),
          maxCommandDelayMs(maxCommandDelayMs),
          timeoutMs(timeout_ms)
    {}
    quint16 stopDelayMs;
    quint16 minCommandDelayMs;
    quint16 maxCommandDelayMs;
    quint16 timeoutMs;
};

struct DeviceDescription
{
    DeviceDescription(const QPixmap& pic, const QString& desc, const QString& link) :
        Photo(pic),
        Description(desc),
        Link(link) {}
    QPixmap Photo;
    QString Description;
    QString Link;
};

struct DeviceModel
{
    DeviceModel();
    DeviceModel(quint16 id, const QString& name, const DeviceDelays &delays, const QMap<quint16, CommandSettings> &cmdBuilders);
    quint16 id;
    QString name;
    DeviceDelays delays;
    QMap<quint16, CommandSettings> commands;
};
