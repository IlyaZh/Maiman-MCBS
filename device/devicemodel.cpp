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

DeviceModel::~DeviceModel() {
    //    if(m_Delays != nullptr) delete m_Delays;
    //    if(m_Description != nullptr) delete m_Description;

    //    for(auto item : commands) delete item;

    //    for (DeviceBinaryOption* item : *m_BinaryOptions) delete item;
    //    if(m_BinaryOptions != nullptr) delete m_BinaryOptions;
}

//Device* DeviceModel::createDevice(quint8 addr, QObject* parent) const
//{
//    return new Device(addr, *this, parent);
//}
