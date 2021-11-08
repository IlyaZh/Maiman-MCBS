#include "devicecommand.h"
#include "globals.h"
#include <QtMath>

const quint16 DeviceCommand::MAX_COM_INTERVAL_COUNTER = 100;

DeviceCommand::DeviceCommand(QString code, const QString& unit, double divider, quint8 interval, quint8 tol, bool isTemperatureFlag, quint16 maxInterval, QObject *parent) :
    QObject(parent),
    value(0),
    iValue(0)
{
    this->codeStr = code;
    this->code = code.toUInt(nullptr, 16);
    this->interval = (interval < MAX_COM_INTERVAL_COUNTER) ? interval : MAX_COM_INTERVAL_COUNTER;
    if(this->interval < 1) this->interval = 1;
    this->Divider = divider;
    this->tol = tol;
    this->isTemperatureFlag = isTemperatureFlag;
    this->unit = unit;
    this->maxInterval = maxInterval;
    rawValue.setValue(-1);
    tick = 0;

//    setTemperatureUnit(settings.getTemperatureSymbol());
}

DeviceCommand* DeviceCommand::copy() {
    return new DeviceCommand(codeStr, unit, Divider, interval, isTemperatureFlag, maxInterval);
}

double DeviceCommand::convertCelToFar(double val) {
    return val * 9.0 / 5.0 + 32.0;
}

double DeviceCommand::convertFarToCel(double val) {
    return (val - 32.0) * 5.0 / 9.0;
}

void DeviceCommand::setTemperatureUnit(const QString& unit) {
    temperatureUnit = unit;
    emit valueChanged(getValue());
}

QString DeviceCommand::getTemperatureUnit() const {
    return temperatureUnit;
}

QString DeviceCommand::getCodeStr() const {
    return codeStr;
}

quint16 DeviceCommand::getCode() const {
    return code;
}

double DeviceCommand::getValue() const {
    if(isTemperature() && temperatureUnit == "F") {
        return qRound(convertCelToFar(value)*qPow(10,tol)-0.5)/qPow(10,tol);
    } else {
        return qRound(value*qPow(10,tol)-0.5)/qPow(10,tol);
    }
}

qint16 DeviceCommand::getIValue() const {
    if(isTemperature() && temperatureUnit == "F") {
        return qRound(convertCelToFar(value)*getDivider());
    } else {
        return iValue;
    }
}

quint16 DeviceCommand::getRawValue() const { // it was int
    return static_cast<quint16>(rawValue.toUInt());
}

double DeviceCommand::getDivider() const {
    return Divider;
}

quint8 DeviceCommand::getTol() const {
    return tol;
};

bool DeviceCommand::isTemperature() const {
    return isTemperatureFlag;
}

QString DeviceCommand::getUnit() const {
    if(isTemperature()) {
        return unit+temperatureUnit;
    } else {
        return unit;
    }
}

quint16 DeviceCommand::getRawFromValue(double value) {
    return static_cast<quint16>(qRound(value * Divider));
}


bool DeviceCommand::isSignedValue() const {
    return false;
}

// SLOTS are declared below

void DeviceCommand::setRawValue(quint16 iValue) {
//    if(rawValue.toUInt() != iValue) {
        rawValue.setValue(iValue);
        this->iValue = static_cast<quint16>(rawValue.toUInt());
        this->value = rawValue.toDouble() / Divider;

        emit valueChanged(getValue());
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

quint8 DeviceCommand::getInterval() const {
    return interval;
}

void DeviceCommand::resetInterval() {
    tick = interval;
}
