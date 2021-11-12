#include "commandsettings.h"
#include <QtMath>
#include <QDebug>
#include <QLocale>
#include <algorithm>

CommandSettings::CommandSettings(quint16 code, const QString& unit, double divider, quint8 tolerance, uint interval, bool isSigned, bool isTemperature) :
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

const int DevCommand::maxLogValues = 100;

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
    m_strValue = QString::number(0, 'f', static_cast<int>(config.m_tolerance));
    m_logValues.fill(0, maxLogValues);
}

void DevCommand::changeTemperatureUnit(Const::TemperatureUnitId id) {
    // TODO: make a function's implementation
    if(config.m_isTemperature && m_tempId != id) {
        m_tempId = id;
        emit updatedUnit(Const::TemperatureUnits.at(static_cast<int>(Const::TemperatureUnitId::Celsius)));
    }
}

quint16 DevCommand::code() const {
    return config.m_code;
}

QString DevCommand::unit() const {
    return config.m_unit;
}

double DevCommand::divider() const {
    return config.m_divider;
}

int DevCommand::tolerance() const {
    return static_cast<int>(config.m_tolerance);
}

quint16 DevCommand::getRawFromValue(double value) const {
    return static_cast<quint16>(qRound(value*config.m_divider-0.5));
}

bool DevCommand::isSigned() const {
    return config.m_isSigned;
}

bool DevCommand::isTemperature() const {
    return config.m_isTemperature;
}

double DevCommand::valueDouble() const {
    return m_value;
}

int DevCommand::valueInt() const {
    return m_iValue;
}

QString DevCommand::valueStr() const {
    return m_strValue;
}

uint DevCommand::interval() const {
    return config.m_interval;
}

const QVector<double>& DevCommand::historyValues() const {
    return m_logValues;
}

double DevCommand::avgValue() const {
    return m_cmdSum / static_cast<double>(maxLogValues);
}

double DevCommand::maxValue() const {
    return *std::max_element(m_logValues.begin(), m_logValues.end());
}

double DevCommand::minValue() const {
    return *std::min_element(m_logValues.begin(), m_logValues.end());
}


// public slots
void DevCommand::setFromDevice(quint16 value) {
    m_rawValue = value;

    double d = 0;
    if(isSigned()) {
        d = static_cast<double>(static_cast<int16_t>(m_rawValue));
        m_value = qRound(d/config.m_divider*qPow(10,config.m_tolerance)-0.5)/qPow(10,config.m_tolerance);
    } else {
        d = static_cast<double>(static_cast<quint16>(m_rawValue));
        m_value = qRound(d/config.m_divider*qPow(10,config.m_tolerance)-0.5)/qPow(10,config.m_tolerance);
    }
    m_iValue = static_cast<int>(m_value);
    m_strValue = QString::number(m_value, 'f', static_cast<int>(config.m_tolerance));

    if(m_cmdIt < m_logValues.size())
        m_cmdSum -= m_logValues.at(m_cmdIt);
    m_logValues[m_cmdIt++] = m_value;
    m_cmdSum += m_value;
    if(m_cmdIt >= m_logValues.size()) m_cmdIt = 0;

    emit updatedValue();
}

void DevCommand::setFromWidget(int value) {
    qDebug() << "DevCommand::setFromWidget" << config.m_code << getRawFromValue(static_cast<double>(value));
    emit sendValueSignal(config.m_code, getRawFromValue(static_cast<double>(value)));
}

void DevCommand::setFromWidget(double value) {
    emit sendValueSignal(config.m_code, getRawFromValue(value));
}
