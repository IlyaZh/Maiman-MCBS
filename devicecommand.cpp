#include "devicecommand.h"
#include "globals.h"

const quint16 DeviceCommand::MAX_COM_INTERVAL_COUNTER = 100;

DeviceCommand::DeviceCommand(QString code, QString unit, double divider, quint8 interval, bool isTemperatureFlag, QObject *parent) : QObject(parent)
{
    this->Code = code;
    this->interval = (interval < MAX_COM_INTERVAL_COUNTER) ? interval : MAX_COM_INTERVAL_COUNTER;
    if(this->interval < 1) this->interval = 1;
    this->Divider = divider;
    this->isTemperatureFlag = isTemperatureFlag;
    this->unit = unit;
    rawValue.setValue(-1);
    tick = 0;

//    setTemperatureUnit(settings.getTemperatureSymbol());
}

double DeviceCommand::convertCelToFar(double val) {
    return val * 9.0 / 5.0 + 32.0;
}

double DeviceCommand::convertFarToCel(double val) {
    return (val - 32.0) * 5.0 / 9.0;
}

void DeviceCommand::setTemperatureUnit(QString unit) {
    temperatureUnit = unit;
    emit valueChanged();
}

QString DeviceCommand::getTemperatureUnit() {
    return temperatureUnit;
}

QString DeviceCommand::getCode() {
    return Code;
}

double DeviceCommand::getValue() {
    if(isTemperature() && temperatureUnit == "F") {
        return convertCelToFar(value);
    } else {
        return value;
    }
}

int DeviceCommand::getIValue() {
    if(isTemperature() && temperatureUnit == "F") {
        return qRound(convertCelToFar(value)*getDivider());
    } else {
        return iValue;
    }
}

int DeviceCommand::getRawValue() {
    return static_cast<quint16>(rawValue.toUInt());
}

double DeviceCommand::getDivider() {
    return Divider;
}

bool DeviceCommand::isTemperature() {
    return isTemperatureFlag;
}

QString DeviceCommand::getUnit() {
    if(isTemperature()) {
        return unit+temperatureUnit;
    } else {
        return unit;
    }
}

int DeviceCommand::getRawFromValue(double value) {
    return qRound(value * Divider);
}

bool DeviceCommand::isSignedValue() {
    return false;
}

// SLOTS are declared below

void DeviceCommand::setRawValue(quint16 iValue) {
//    if(rawValue.toUInt() != iValue) {
        rawValue.setValue(iValue);
        this->iValue = static_cast<quint16>(rawValue.toUInt());
        this->value = rawValue.toDouble() / Divider;

        emit valueChanged();
//    }
}

bool DeviceCommand::needToRequest() {
    bool result = false;
    if((tick % interval) == 0) {
        result = true;
    }

    tick++;
    if(tick > MAX_COM_INTERVAL_COUNTER) tick = 0;

    return result;
}

void DeviceCommand::resetInterval() {
    tick = interval;
}
