#include "device.h"
#include <QMapIterator>

Device::Device(quint16 id, quint8 addr, QString name, DeviceDelays *delays, QVector<DevCommand*> *commands, QObject *parent)
    : QObject(parent),
    m_addr(addr),
    m_Id(id),
    m_Name(name),
    m_Delays(delays)
{
    m_Commands = new QMap<quint16, DevCommand*>();
    for(DevCommand* cmd : *commands) {
        m_Commands->insert(cmd->code(), cmd);
    }

    createCommandsRequests();

    timeoutTimer = new QTimer();
    timeoutTimer->singleShot(m_Delays->timeoutMs(), this, SLOT(timeout()));
}

void Device::dataIncome(quint16 reg, quint16 value) {
    for(DevCommand* cmd : *m_Commands) {
        if(cmd->code() == reg) {
            timeoutTimer->stop();
            m_isLink = true;
            timeoutTimer->singleShot(m_Delays->timeoutMs(), this, SLOT(timeout()));

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
        DevicePollRequest* request = *(m_cmdReqItt);
        m_cmdReqItt++;
        if(request->isRequestReady()) {
            return request;
        }
    }
    return nullptr;
}

bool Device::isLink() {
    bool m_isLink;
}

//QVector<const DeviceCommand*>* Device::getCommands() {
//    QVector<const DeviceCommand*>* vector = new QVector<const DeviceCommand*>(m_Commands->size());


//    QMap<quint16, DeviceCommand*>::iterator i;
//    for(i = m_Commands->begin(); i != m_Commands->end(); i++)
//        vector->append(qAsConst(*i));

//    return vector;
//}

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
    int count = 0;
    DevCommand* cmd = nullptr;
    DevCommand* prevCmd = nullptr;

    QMapIterator<quint16, DevCommand*> i(*m_Commands);
    while(i.hasNext()) {
        if(cmd != nullptr)
            prevCmd = cmd;

        i.next();
        cmd = i.value();
        if(startCode == -1) {
            startCode = cmd->code();
            count++;
        } else {
            if(prevCmd->code()+1 == cmd->code()) {
                count++;
            } else {
               m_cmdRequests.append(new DevicePollRequest(m_addr, startCode, count));
               qDebug() << "DevicePollRequest addrr = " << m_addr << ", code = " << startCode << ", count = " << count;
               startCode = cmd->code();
               count = 1;
            }
        }
    }
    m_cmdReqItt = m_cmdRequests.begin();
}

void Device::timeout() {
    m_isLink = false;

}
