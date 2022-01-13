#include "devicecondition.h"
#include "device/commandsettings.h"
#include "devicewidget.h"

DeviceCondition::DeviceCondition(const QMap<quint16, QSharedPointer<DevCommand>>& commands,const QVector <Led>& Leds, QLabel* label,QObject *parent)
    : QObject(parent),
    m_commands(commands),
    m_Leds(Leds),
    m_Label(label)
{
    for(int idleds = 0; idleds < m_Leds.count(); ++idleds) {
        auto led = m_Leds.at(idleds);
        for(int idled=0; idled < led.ledMasks.count(); ++idled){
            auto ledmask = led.ledMasks.at(idled);
            auto cmd = m_commands.value(ledmask.code);
            connect(cmd.get(), &DevCommand::updatedValue,this,[this, cmd, ledmask](){
                setStateText(cmd->valueInt(), ledmask);
            });
        }
    }
}

void DeviceCondition::setStateText(uint value, LedMask led){
    QString msg = led.msg;
    if (led.code == 4)
        msg = "";
    if ((value & led.mask) != 0){
        if (!m_Label->text().contains(msg))
            m_states.append(msg);
    }
    else{
        if (m_Label->text().contains(msg))
            m_states.removeOne(msg);
    }
    m_Label->setText(m_states.join("; "));
}
