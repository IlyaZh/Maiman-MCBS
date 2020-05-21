#ifndef DEVICEBINARYOPTION_H
#define DEVICEBINARYOPTION_H

#include <QObject>

class DeviceBinaryOption {
public:
    DeviceBinaryOption(QString name, quint16 code, quint16 onCommand, quint16 offCommand, quint16 mask) : m_Code(code), m_OnCommand(onCommand), m_OffCommand(offCommand), m_Mask(mask), m_Label(name) {}
    QString name() { return m_Label; }
    quint16 code() { return m_Code; }
    quint16 onCommand() { return m_OnCommand; }
    quint16 offCommand() { return m_OffCommand; }
    quint16 mask() { return m_Mask; }

private:
    quint16 m_Code;
    quint16 m_OnCommand;
    quint16 m_OffCommand;
    quint16 m_Mask;
    QString m_Label;
};

#endif // DEVICEBINARYOPTION_H
