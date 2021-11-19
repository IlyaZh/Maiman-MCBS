#include "device.h"
#include <QMapIterator>

//Device::Device(quint16 id, quint8 addr, const QString& name, const DeviceDelays &delays, const QVector<DevCommandBuilder*> &commandsBld, QObject *parent)
Device::Device(quint8 addr, const DeviceModel& config, QObject *parent)
    : QObject(parent),
      m_addr(addr),
      m_Id(config.id),
      m_Name(config.name),
      m_Delays(config.delays)
    //    m_timer(new QTimer(this))
{
    for(const auto &cmdBuilder : config.commands) {
        auto command = new DevCommand(cmdBuilder);
        m_Commands.insert(command->code(), QSharedPointer<DevCommand>(command));

        connect(command, &DevCommand::sendValueSignal, this, &Device::dataFromCommand);
    }

    createCommandsRequests();

    //    m_timer->setInterval(Const::ComPortTimeout);
    //    m_timer->setSingleShot(true);
    //    connect(m_timer, &QTimer::timeout, this, &Device::timeout);

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
        emit linkChanged(m_isLink);
        //        m_timer->stop();
        //        if(m_timeoutEnabled)
        //            m_timer->start();

        cmd->setFromDevice(value);
    }
}

//void Device::destroy() {
//    this->disconnect();
//}

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
    m_cmdRequests.clear();
    for(auto& command : m_Commands.values()) {
        disconnect(command.get(), &DevCommand::sendValueSignal, this, &Device::dataFromCommand);
    }
    emit linkChanged(m_isLink);
}

//void Device::enableTimeout(bool enable) {
//    m_timeoutEnabled = enable;
//    if(!m_timeoutEnabled) {
//        m_timer->stop();
//    }
//}

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
    if (m_isLink)
        emit dataToModel(m_addr, reg, value);
}

//void Device::timeout() {
//    m_isLink = false;
//    emit link(m_isLink);
//}

