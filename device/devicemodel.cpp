#include "devicemodel.h"

DeviceModel::DeviceModel(quint16 id, QString name, DeviceDelays *delays, QVector<DevCommand*> *commands) :
    m_Id(id),
    m_Name(name),
    m_Delays(delays),
//    m_Description(description),
    m_Commands(commands)
//    m_BinaryOptions(binaryOptions)
{}

DeviceModel::~DeviceModel() {
    if(m_Delays != nullptr) delete m_Delays;
//    if(m_Description != nullptr) delete m_Description;

    for(DevCommand* item : *m_Commands) delete item;
    if(m_Commands != nullptr) delete m_Commands;

//    for (DeviceBinaryOption* item : *m_BinaryOptions) delete item;
//    if(m_BinaryOptions != nullptr) delete m_BinaryOptions;
}

Device* DeviceModel::createDevice(quint8 addr)
{
    DevCommand* pDevice = nullptr;
    QVector<DevCommand*> *commands = new QVector<DevCommand*>();

    pDevice = new Device(m_Id, addr, m_Name, m_Delays, commands);
    for(DevCommand* cmd : *m_Commands) {
        commands->append(cmd->copy());
    }
    return pDevice;
}

QString DeviceModel::name() { return m_Name; }

quint16 DeviceModel::id() { return m_Id; }
