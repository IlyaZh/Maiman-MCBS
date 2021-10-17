#include "commandsettings.h"
#include <QtMath>
#include <QDebug>
#include <QLocale>

CommandSettings::CommandSettings(quint16 code, QString unit, double divider, quint8 tolerance, uint interval, bool isSigned, bool isTemperature) :
    m_code(code),
    m_unit(unit),
    m_divider(divider),
    m_tolerance(tolerance),
    m_isSigned(isSigned),
    m_isTemperature(isTemperature),
    m_interval(interval)
{

}

// static methods

double DevCommand::convertCelToFar(double value) {
    return value * 9.0 / 5.0 + 32.0;
}

double DevCommand::convertFarToCel(double value) {
    return (value - 32.0) * 5.0 / 9.0;
}

// public methods
DevCommand::DevCommand(const CommandSettings& conf) :
    config(conf)
{
    m_rawValue = 0;
}

quint16 DevCommand::code() {
    return config.m_code;
}

QString DevCommand::unit() {
    return config.m_unit;
}

double DevCommand::divider() {
    return config.m_divider;
}

int DevCommand::tolerance() {
    return static_cast<int>(config.m_tolerance);
}

quint16 DevCommand::getRawFromValue(double value) {
    return static_cast<quint16>(qRound(value*config.m_divider-0.5));
}

bool DevCommand::isSigned() {
    return config.m_isSigned;
}

double DevCommand::valueDouble() {
    return m_value;
}

uint DevCommand::valueUInt() {
    return m_rawValue;
}

QString DevCommand::valueStr() {
    return m_strValue;
}

uint DevCommand::interval() {
    return config.m_interval;
}


// public slots
void DevCommand::setFromDevice(quint16 value) {
    m_rawValue = value;

    double d = 0;
    if(isSigned()) {
        d = static_cast<double>((int16_t) m_rawValue);
        m_value = qRound(d/config.m_divider*qPow(10,config.m_tolerance)-0.5)/qPow(10,config.m_tolerance);
    } else {
        d = static_cast<double>(m_rawValue);
        m_value = qRound(d/config.m_divider*qPow(10,config.m_tolerance)-0.5)/qPow(10,config.m_tolerance);
    }
    m_strValue = QString::number(d, 'f', static_cast<int>(config.m_tolerance));
    emit updatedValue();
}

void DevCommand::setFromWidget(int value) {
    emit sendValueSignal(config.m_code, getRawFromValue(static_cast<double>(value)));
}

void DevCommand::setFromWidget(double value) {
    emit sendValueSignal(config.m_code, getRawFromValue(value));
}
