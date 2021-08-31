#include "devcommand.h"
#include <QtMath>

// DevCommandBuilder class
CommandSettings::CommandSettings(quint16 code, QString unit, double divider, quint8 tolerance, uint interval, bool isSigned, bool isTemperature) :
    code(code),
    unit(unit),
    divider(divider),
    tolerance(tolerance),
    isSigned(isSigned),
    isTemperature(isTemperature),
    interval(interval)
{

}

//DevCommand* CommandSettings::makeCommand(Device* pDevice) {
//    return new DevCommand(pDevice, *this);
//}

//quint16 DeviceConfig::code() const {
//    return config.code;
//}

//uint DeviceConfig::interval() const {
//    return config.interval;
//}

// DevCommand class
// static methods

double DevCommand::convertCelToFar(double value) {
    return value * 9.0 / 5.0 + 32.0;
}

double DevCommand::convertFarToCel(double value) {
    return (value - 32.0) * 5.0 / 9.0;
}

// public methods
DevCommand::DevCommand(Device* device, const CommandSettings& conf, QObject *parent) :
    QObject(parent),
    config(conf),
    m_device(device)
{
    m_rawValue = 0;
}

//void DevCommand::execute(int value) {
//    quint16 inValue = 0;
//    if(isSigned()) {
//        inValue = (quint16) value;
//    } else {
//        inValue = value;
//    }

//    m_device->dataOutcome(config.code, inValue);
//}

//void DevCommand::execute(double value) {
//    quint16 inValue = qRound(value*config.divider);
//    if(isSigned()) {
//        inValue = static_cast<quint16>((qint16) qRound(value*config.divider));
//    } else {
//        inValue = static_cast<quint16>(qRound(value*config.divider));
//    }

//    m_device->dataOutcome(config.code, inValue);
//}

/*quint16 DevCommand::code() {
    return config.code;
}

QString DevCommand::unit() {
    return config.unit;
}

double DevCommand::divider() {
    return config.divider;
}

quint8 DevCommand::tolerance() {
    return config.tolerance;
}

uint DevCommand::interval() {
    return config.interval;
}*/

//uint DevCommand::stepInterval() {
//    return m_stepInterval;
//}

//bool DevCommand::nextInterval() {
//    m_stepInterval++;
//    bool state = (m_stepInterval == config.interval);

//    if(state) {
//        m_stepInterval = 0;
//    }
//    return state;
//}

quint16 DevCommand::getRawFromValue(double value) {
    return static_cast<quint16>(qRound(value*config.divider-0.5));
}

bool DevCommand::isSigned() {
    return config.isSigned;
}

double DevCommand::value() {
    return m_value;
}


// public slots
void DevCommand::setRawValue(quint16 value) {
    m_rawValue = value;

    if(isSigned()) {
        double d = static_cast<double>((int16_t) m_rawValue);
        m_value = qRound(d/config.divider*qPow(10,config.tolerance)-0.5)/qPow(10,config.tolerance);
    } else {
        double d = static_cast<double>(m_rawValue);
        m_value = qRound(d/config.divider*qPow(10,config.tolerance)-0.5)/qPow(10,config.tolerance);
    }


    /*QVariant variant(value);
    m_rawValue = value;

    int iValue = (isSigned()) ? static_cast<qint16>(variant.toInt()) : static_cast<quint16>(variant.toUInt());
    double dValue = (static_cast<double>(iValue) / divider());
    double power = qPow(10, config.tolerance);;
    dValue *= power;
    dValue = qRound(dValue);
    dValue /= power;
    m_value = dValue;*/
//    QString tempStr;
//    tempStr.number(dValue, 'g', tolerance());

//    emit newValue(config.code, tempStr.toDouble());
}
