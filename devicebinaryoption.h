#ifndef DEVICEBINARYOPTION_H
#define DEVICEBINARYOPTION_H

#include <QObject>
#include <QAbstractButton>
#include <QCheckBox>

#include "device/devicecommand.h"

class DeviceBinaryOption {
public:
    DeviceBinaryOption(QString name, DeviceCommand* code, quint16 onCommand, quint16 offCommand, quint16 mask);
    DeviceBinaryOption* copy();
    QString name();
    const DeviceCommand* command();
    quint16 onCommand();
    quint16 offCommand();
    quint16 mask();

protected:
    QString m_Name;
    DeviceCommand* m_Code;
    quint16 m_OnCommand;
    quint16 m_OffCommand;
    quint16 m_Mask;
    QString m_Label;
};

//class DeviceBinaryButton : DeviceBinaryOption {
//public:
//    DeviceBinaryButton(QString name, DeviceCommand* code, quint16 onCommand, quint16 offCommand, quint16 mask, QAbstractButton* button)
//        : DeviceBinaryOption(name, code, onCommand, offCommand, mask), m_button(button)
//    {}
//    const QAbstractButton* button() { return m_button; }
//    ~DeviceBinaryButton() { if(m_button) delete m_button; }

//signals:
//    void clicked(bool newState);

//protected:
//    QAbstractButton* m_button;

//};

#endif // DEVICEBINARYOPTION_H
