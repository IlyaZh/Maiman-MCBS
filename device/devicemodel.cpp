#include "devicemodel.h"

DeviceModel::DeviceModel() :
    id(0),
    name("")
{
}

DeviceModel::DeviceModel(quint16 id, const QString& name, const DeviceDelays &delays, const QMap<quint16, CommandSettings> &cmdBuilders) :
    id(id),
    name(name),
    delays(delays),
    commands(cmdBuilders)
{}
