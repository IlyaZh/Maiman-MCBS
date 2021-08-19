#include "device.h"
#include <QMapIterator>

int Device::counter = 0;

//Device::Device(quint16 id, quint8 addr, const QString& name, const DeviceDelays &delays, const QVector<DevCommandBuilder*> &commandsBld, QObject *parent)
Device::Device(quint8 addr, const DeviceModel& config, QObject *parent)
    : QObject(parent),
    m_addr(addr),
    m_Id(config.id),
    m_Name(config.name),
    m_Delays(config.delays)
{
    for(auto cmdBldr : config.commands) {
        m_Commands.insert(cmdBldr->code(), cmdBldr->makeCommand(this));
    }

    createCommandsRequests();

    qDebug() << "Create device" << m_addr << m_Name << m_Id << "Counter =" << ++counter;

//    timeoutTimer = new QTimer();
//    timeoutTimer->singleShot(m_Delays->timeoutMs(), this, SLOT(timeout()));
}

Device::~Device() {
    qDebug() << "Delete device" << m_addr << m_Name << m_Id << "Counter =" << --counter;

    for(auto *item : m_cmdRequests) {
        delete item;
    }
    m_cmdRequests.clear();

    if(m_deviceWidget != nullptr) m_deviceWidget->deleteLater();

}

void Device::dataIncome(quint16 reg, quint16 value) {
    for(DevCommand* cmd : m_Commands) {
        if(cmd->code() == reg) {
//            timeoutTimer->stop();
            m_isLink = true;
            emit link(m_isLink);
//            timeoutTimer->singleShot(m_Delays->timeoutMs(), this, SLOT(timeout()));

            cmd->setRawValue(value);
//            emit dataToView(reg, cmd.va());
        }
    }
}

void Device::dataOutcome(quint16 reg, quint16 value) {
//    DevCommand* devCmd = m_Commands->value(reg, nullptr);
//    if(devCmd != nullptr) {
//        quint16 rawValue = devCmd->getRawFromValue(value);
        emit dataToModel(m_addr, reg, value);
//    }

}

void Device::setWidget(QWidget& widget) {
    if(m_deviceWidget) {
        m_deviceWidget->disconnect();
        m_deviceWidget->deleteLater();
    }

    m_deviceWidget = &widget;
}

void Device::destroy() {
    this->disconnect();
//    this->deleteLater();
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

DevicePollRequest* Device::nextPollRequest() {
    if(m_cmdReqItt == m_cmdRequests.end())  m_cmdReqItt = m_cmdRequests.begin();
    while(m_cmdReqItt != m_cmdRequests.end()) {
        DevicePollRequest* request = *m_cmdReqItt;
        m_cmdReqItt++;
        if(request->isRequestReady()) {
            return request;
        }
    }
    return nullptr;
}

bool Device::isLink() {
    return m_isLink;
}

void Device::clearLink() {
    m_isLink = false;
    emit link(m_isLink);
}

void Device::addWidget(DeviceWidget& widget) {

}

void Device::removeWidget(DeviceWidget& widget) {

}

/*QVector<const DevCommand*>* Device::getCommands() {
    QVector<const DevCommand*>* vector = new QVector<const DevCommand*>(m_Commands.size());

    QMap<quint16, QPointer<DevCommand>>::iterator i;
    for(i = m_Commands.begin(); i != m_Commands.end(); i++)
        vector->append(qAsConst(i.value()));

    return vector;
}*/

/*void Device::connectWidget(DeviceWidget* widget, int code) {
    if(m_deviceWidgets.contains(code)) {}

    if(!m_deviceWidgets.contains(widget)) {
        m_deviceWidgets.append(widget);
//        connect(widget, SIGNAL(dataChanged(quint16, int)), this, SIGNAL(dataChanged));
//        connect(widget, SIGNAL(dataChanged(int)), [this](){

//        });
    }
}

void Device::disconnectWidget(DeviceWidget* widget, int code) {
    if(code == -1) {
        for() {

        }
    } else {
        m_deviceWidgets.remove(code, widget)
    }


    if(m_deviceWidgets.contains(widget)) {
        widget->disconnect();
        m_deviceWidgets.removeOne(widget);
    }
}*/


// private methods
void Device::createCommandsRequests() {
    m_cmdRequests.clear();
    int startCode = -1;
    uint minInterval = 1;
    int count = 0;
    DevCommand* cmd = nullptr;
    DevCommand* prevCmd = nullptr;
    bool bHasCommands = false;

    QMapIterator<quint16, DevCommand*> i(m_Commands);
    while(i.hasNext()) {
        bHasCommands = true;
        if(cmd != nullptr)
            prevCmd = cmd;

        i.next();
        cmd = i.value();
        if(startCode == -1) {
            startCode = cmd->code();
            minInterval = cmd->interval();
            count++;
        } else {
            if(prevCmd->code()+1 == cmd->code()) {
                count++;
                if(minInterval > cmd->interval()) minInterval = cmd->interval();
            } else {
               m_cmdRequests.append(new DevicePollRequest(m_addr, startCode, count, minInterval));
//               qDebug() << "DevicePollRequest addrr = " << m_addr << ", code = " << startCode << ", count = " << count;
               startCode = cmd->code();
               minInterval = cmd->interval();
               count = 1;
            }
        }
    }
    if(bHasCommands) {
        m_cmdRequests.append(new DevicePollRequest(m_addr, startCode, count, minInterval));
//        qDebug() << "DevicePollRequest addrr = " << m_addr << ", code = " << startCode << ", count = " << count;
    }
    m_cmdReqItt = m_cmdRequests.begin();
}

//void Device::timeout() {
//    m_isLink = false;
//}
