#include "devicebinaryoption.h"

DeviceBinaryOption::DeviceBinaryOption(QString name, DeviceCommand* code, quint16 onCommand, quint16 offCommand, quint16 mask)
    : m_Name(name), m_Code(code), m_OnCommand(onCommand), m_OffCommand(offCommand), m_Mask(mask), m_Label(name)
{

}
DeviceBinaryOption* DeviceBinaryOption::copy() {
    return new DeviceBinaryOption(m_Name, m_Code, m_OnCommand, m_OffCommand, m_Mask);
}

QString DeviceBinaryOption::name() {
    return m_Label;
}

const DeviceCommand* DeviceBinaryOption::command() {
    return m_Code;
}

quint16 DeviceBinaryOption::onCommand() {
    return m_OnCommand;
}

quint16 DeviceBinaryOption::offCommand() {
    return m_OffCommand;
}

quint16 DeviceBinaryOption::mask() {
    return m_Mask;
}
