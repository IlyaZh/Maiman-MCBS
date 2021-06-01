#include "devicemodel.h"

DeviceModel::DeviceModel(quint16 id, QString name, DeviceDelays *delays, QVector<DevCommandBuilder*> *cmdBuilders) :
    m_Id(id),
    m_Name(name),
    m_Delays(delays),
//    m_Description(description),
    m_Commands(cmdBuilders)
//    m_BinaryOptions(binaryOptions)
{}

DeviceModel::~DeviceModel() {
    if(m_Delays != nullptr) delete m_Delays;
//    if(m_Description != nullptr) delete m_Description;

    for(DevCommandBuilder* item : *m_Commands) delete item;
    if(m_Commands != nullptr) delete m_Commands;

//    for (DeviceBinaryOption* item : *m_BinaryOptions) delete item;
//    if(m_BinaryOptions != nullptr) delete m_BinaryOptions;
}

Device* DeviceModel::createDevice(quint8 addr)
{
    return new Device(m_Id, addr, m_Name, m_Delays, m_Commands);
}

QString DeviceModel::name() { return m_Name; }

quint16 DeviceModel::id() { return m_Id; }
