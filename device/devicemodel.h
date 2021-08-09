#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QObject>
#include "../device/devcommand.h"
#include "device.h"
#include "globals.h"

class Device;
class DevCommand;
class DevCommandBuilder;

class DeviceDelays {
public:
    static const quint16 COM_COMMAND_MIN_SEND_DELAY = 50;
    static const quint16 COM_COMMAND_MAX_SEND_DELAY = 1000;
    static const quint16 COM_STOP_DELAY_MS = 300;

    DeviceDelays(quint16 stopDelayMs = COM_STOP_DELAY_MS, quint16 minCommandDelayMs = COM_COMMAND_MIN_SEND_DELAY, quint16 maxCommandDelayMs = COM_COMMAND_MAX_SEND_DELAY, quint16 timeout_ms = COM_PORT_TIMEOUT)
        : m_stopDelayMs(stopDelayMs),
          m_minCommandDelayMs(minCommandDelayMs),
          m_maxCommandDelayMs(maxCommandDelayMs),
          m_timeoutMs(timeout_ms)
    {}
    quint16 stopDelayMs() { return m_stopDelayMs; }
    quint16 minCommandDelayMs() { return m_minCommandDelayMs; }
    quint16 maxCommandDelayMs() { return m_maxCommandDelayMs; }
    quint16 timeoutMs() { return m_timeoutMs; }
private:
    quint16 m_stopDelayMs;
    quint16 m_minCommandDelayMs;
    quint16 m_maxCommandDelayMs;
    quint16 m_timeoutMs;
};

class DeviceDescription
{
public:
    DeviceDescription(QPixmap* pic, QString desc, QString link) : m_Photo(pic), m_Description(desc), m_Link(link) {}
    ~DeviceDescription() {}
    const QPixmap& photo() { return *m_Photo; }
    QString description() { return m_Description; }
    QString link() { return m_Link; }
private:
    QPixmap* m_Photo;
    QString m_Description;
    QString m_Link;
};

class DeviceModel
{
public:
    DeviceModel(quint16 id, QString name, DeviceDelays *delays, QVector<DevCommandBuilder*> *cmdBuilders);
    ~DeviceModel();
    Device* createDevice(quint8 addr, QObject *parent = nullptr);
    QString name();
    quint16 id();

private:
    quint16 m_Id;
    QString m_Name;
    DeviceDelays* m_Delays;
//    DeviceDescription* m_Description;
    QVector<DevCommandBuilder*> *m_Commands;
//    QVector<DeviceBinaryOption*> *m_BinaryOptions;
};

#endif // DEVICEMODEL_H
