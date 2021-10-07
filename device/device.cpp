#include "device.h"
#include <QMapIterator>

//Device::Device(quint16 id, quint8 addr, const QString& name, const DeviceDelays &delays, const QVector<DevCommandBuilder*> &commandsBld, QObject *parent)
Device::Device(quint8 addr, const DeviceModel& config, QObject *parent)
    : QObject(parent),
    m_addr(addr),
    m_Id(config.id),
    m_Name(config.name),
    m_Delays(config.delays)
{
    for(const auto &cmdBuilder : config.commands) {
        auto command = new DevCommand(cmdBuilder);
        m_Commands.insert(command->code(), QSharedPointer<DevCommand>(command));

        connect(command, &DevCommand::sendValueSignal, this, &Device::dataFromCommand);
    }

    createCommandsRequests();

    qDebug() << "Create device" << m_addr << m_Name << m_Id << "Counter";
}

Device::~Device() {
    qDebug() << "Delete device" << m_addr << m_Name << m_Id;

    m_cmdRequests.clear();
    m_Commands.clear();
}

void Device::dataIncome(quint16 reg, quint16 value) {
    for(auto &cmd : qAsConst(m_Commands)) {
        if(cmd->code() == reg) {
            m_isLink = true;
            emit link(m_isLink);

            cmd->setRawValue(value);
        }
    }
}


void Device::destroy() {
    this->disconnect();
}

QString Device::name() {
    return m_Name;
}

quint16 Device::id() {
    return m_Id;
}

quint8 Device::addr() {
    return m_addr;
}

const DevicePollRequest Device::nextPollRequest() {
    if(m_cmdReqIt >= m_cmdRequests.size()) m_cmdReqIt = 0;
    while(m_cmdReqIt < m_cmdRequests.size()) {
    DevicePollRequest request = m_cmdRequests.at(m_cmdReqIt);
        m_cmdReqIt++;
        if(request.isRequestReady()) {
            return request;
        }
    }
    return DevicePollRequest(0,0,0);
}

bool Device::isLink() {
    return m_isLink;
}

void Device::clearLink() {
    m_isLink = false;
    emit link(m_isLink);
}

const QMap<quint16, QSharedPointer<DevCommand>> Device::commands() {
    return m_Commands;
}


// private methods
void Device::createCommandsRequests() {
    m_cmdRequests.clear();
    int startCode = -1;
    uint minInterval = 1;
    int count = 0;

    QMapIterator<quint16, QSharedPointer<DevCommand>> i(m_Commands);

    qDebug() << "createCommandsRequests " << m_Commands.keys();

    for(auto it = m_Commands.begin(); it != m_Commands.end(); ++it) {
         auto cmd = it.value();
        if(startCode == -1) {
            startCode = cmd->code();
            minInterval = cmd->interval();
            count++;
        } else {
            if((startCode+count) == cmd->code()) {
                count++;
                if(minInterval > cmd->interval()) minInterval = cmd->interval();
            } else {
               m_cmdRequests.append(DevicePollRequest(m_addr, startCode, count, minInterval));
               startCode = cmd->code();
               minInterval = cmd->interval();
               count = 1;
            }
        }
    }
        if(startCode != -1) {
            m_cmdRequests.append(DevicePollRequest(m_addr, startCode, count, minInterval));
        }
        m_cmdReqIt = 0;
}

// private slots

void Device::dataFromCommand(quint16 reg, quint16 value) {
    qDebug() << "DataFromCommand" << reg << value;
    emit dataToModel(m_addr, reg, value);
}

