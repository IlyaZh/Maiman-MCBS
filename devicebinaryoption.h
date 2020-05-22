#ifndef DEVICEBINARYOPTION_H
#define DEVICEBINARYOPTION_H

#include <QObject>
#include <QAbstractButton>
#include <QCheckBox>
#include <QPointer>

#include "devicecommand.h"

class DeviceBinaryOption {
public:
    DeviceBinaryOption(QString name, DeviceCommand* code, quint16 onCommand, quint16 offCommand, quint16 mask) : m_Code(code), m_OnCommand(onCommand), m_OffCommand(offCommand), m_Mask(mask), m_Label(name) {}
    QString name() { return m_Label; }
    const DeviceCommand* command() { return m_Code; }
    quint16 onCommand() { return m_OnCommand; }
    quint16 offCommand() { return m_OffCommand; }
    quint16 mask() { return m_Mask; }

protected:
    QPointer<DeviceCommand> m_Code;
    quint16 m_OnCommand;
    quint16 m_OffCommand;
    quint16 m_Mask;
    QString m_Label;
};

class DeviceBinaryButton : DeviceBinaryOption {
public:
    DeviceBinaryButton(QString name, DeviceCommand* code, quint16 onCommand, quint16 offCommand, quint16 mask, QAbstractButton* button)
        : DeviceBinaryOption(name, code, onCommand, offCommand, mask), m_button(button)
    {}
    const QAbstractButton* button() { return m_button; }
    ~DeviceBinaryButton() { if(m_button) delete m_button; }

signals:
    void clicked(bool newState);

protected:
    QPointer<QAbstractButton> m_button;

};

#endif // DEVICEBINARYOPTION_H
