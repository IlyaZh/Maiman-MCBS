#include "devicesignedcommand.h"

DeviceSignedCommand::DeviceSignedCommand(QString code, QString sUnit, double divider, quint8 cInterval, bool isTemperature, QObject* parent) : DeviceCommand(code, sUnit, divider, cInterval, isTemperature, parent)
{

}

bool DeviceSignedCommand::isSignedValue() {
    return true;
}

int DeviceSignedCommand::getRawValue() {
    return static_cast<qint16>(rawValue.toInt());
}

int DeviceSignedCommand::getRawFromValue(double value) {
    QVariant var;
    var.setValue(qRound(value*Divider));
    return static_cast<quint16>(var.toInt()); // test it. Im not sure about this conversion is correct
}

void DeviceSignedCommand::setRawValue(quint16 inValue) {
    if(rawValue.toUInt() != inValue) {
        rawValue.setValue(inValue);
        this->iValue = static_cast<qint16>(rawValue.toUInt());
        this->value = static_cast<double>(this->iValue) / Divider;

        emit valueChanged();
    }
}
