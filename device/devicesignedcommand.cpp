#include "devicesignedcommand.h"

DeviceSignedCommand::DeviceSignedCommand(const QString& code, const QString& unit, double divider, quint8 interval, quint8 tol, bool isTemperature, quint16 maxInterval, QObject* parent)
    : DeviceCommand(code, unit, divider, interval, tol, isTemperature, maxInterval, parent)
{

}

DeviceSignedCommand* DeviceSignedCommand::copy() {
    return new DeviceSignedCommand(codeStr, unit, Divider, interval, isTemperatureFlag, maxInterval);
}

bool DeviceSignedCommand::isSignedValue() const {
    return true;
}

quint16 DeviceSignedCommand::getRawValue() const {
    return static_cast<quint16>(rawValue.toInt());
}

quint16 DeviceSignedCommand::getRawFromValue(double value) { // it was int
    QVariant var;
    var.setValue(qRound(value*Divider));
    return static_cast<quint16>(var.toInt()); // test it. Im not sure about this conversion is correct
}

void DeviceSignedCommand::setRawValue(quint16 inValue) {
    if(rawValue.toUInt() != inValue) {
        rawValue.setValue(inValue);
        this->iValue = static_cast<qint16>(rawValue.toUInt());
        this->value = static_cast<double>(this->iValue) / Divider;

        emit valueChanged(getValue());
    }
}
