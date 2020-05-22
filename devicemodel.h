#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QObject>
#include <QPointer>
#include "devicecommand.h"
#include "interfaces/devicemodelinterface.h"
#include "devicedescription.h"
#include "devicebinaryoption.h"

class DeviceModel : DeviceModelInterface
{
public:
    DeviceModel(quint16 id, QString name, quint16 stopDelayMs, quint16 minCommandDelayMs, quint16 maxCommandDelayMs);
    void setDescriptionContent(DeviceDescription* deviceDesc);
    void addCommand(DeviceCommand* command);
    void addBinaryOption(DeviceBinaryOption* binOption);

private:
    quint16 m_Id;
    QString m_Name;
    quint16 m_StopDelayMs;
    quint16 m_MinCommandDelayMs;
    quint16 m_MaxCommandDelayMs;
    QPointer<DeviceDescription> devDesc;
    QVector<DeviceCommand*> m_Commands;
    QVector<DeviceBinaryOption*> m_BinaryOptions;
};

#endif // DEVICEMODEL_H
