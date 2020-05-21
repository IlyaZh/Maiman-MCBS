#ifndef DEVICEPARAMETER_H
#define DEVICEPARAMETER_H

#include "devicecommand.h"
#include <QPointer>
#include <QObject>

class DeviceParameter {
public:
    DeviceParameter(QString name, DeviceCommand *min, DeviceCommand *max, DeviceCommand *value, DeviceCommand *realValue)
        : m_Name(name), m_Min(min), m_Max(max), m_Value(value), m_RealValue(realValue)
    {}
    bool hasMin() { return !m_Min.isNull(); }
    bool hasMax() { return !m_Max.isNull(); }
    bool hasValue() { return !m_Value.isNull(); }
    bool hasRealValue() { return !m_RealValue.isNull(); }
    QString name() { return m_Name; }
    DeviceCommand* minCmd() { return m_Min; }
    DeviceCommand* maxCmd() { return m_Max; }
    DeviceCommand* valueCmd() { return m_Value; }
    DeviceCommand* realValueCmd() { return m_RealValue; }
private:
    QString m_Name;
    QPointer<DeviceCommand> m_Min;
    QPointer<DeviceCommand> m_Max;
    QPointer<DeviceCommand> m_Value;
    QPointer<DeviceCommand> m_RealValue;
};

#endif // DEVICEPARAMETER_H
