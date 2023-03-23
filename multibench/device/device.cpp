#include "device.h"
#include <QMapIterator>
#include <QDebug>
#include <QDateTime>

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
    auto cmd = m_Commands.value(reg, nullptr);
    if(cmd) {
        m_connectionPolling.reset();

        m_isLink = true;

        emit linkChanged(m_isLink);

        cmd->setFromDevice(value);


    }
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

std::optional<DevicePollRequest> Device::nextPollRequest(bool unconditional) {
    if(m_cmdReqIt >= m_cmdRequests.size()) m_cmdReqIt = 0;

    if(unconditional or m_connectionPolling.isNeedAction()){
        while(m_cmdReqIt < m_cmdRequests.size()) {
            DevicePollRequest request = m_cmdRequests.at(m_cmdReqIt);
            m_cmdReqIt++;
            if(request.isRequestReady()) {
                return request;
            }
        }
    }
    return std::nullopt;
}

bool Device::isLink() {
    return m_isLink;
}

const QMap<quint16, QSharedPointer<DevCommand>>& Device::commands() {
    return m_Commands;
}

void Device::changeTemperatureUnit(Const::TemperatureUnitId id) {
    for(auto& command : m_Commands) {
        if(command->isTemperature()) {
            command->changeTemperatureUnit(id);
        }
    }
}

void Device::unlink() {
    m_isLink = false;
//    m_cmdRequests.clear();
//    for(auto it = m_Commands.begin(); it != m_Commands.end(); ++it) {
//        disconnect(it.value().get(), &DevCommand::sendValueSignal, this, &Device::dataFromCommand);
//    }
    qDebug() << "Device unlink" << m_addr << QDateTime::currentDateTime().time().toString("mm:ss.zzz");
    emit linkChanged(m_isLink);
}

// private methods
void Device::createCommandsRequests() {
    m_cmdRequests.clear();
    int startCode = -1;
    uint minInterval = 1;
    int count = 0;

    QMapIterator<quint16, QSharedPointer<DevCommand>> i(m_Commands);

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
    if (m_isLink)
        emit dataToModel(m_addr, reg, value);
}
