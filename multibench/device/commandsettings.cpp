#include "commandsettings.h"

#include <QLocale>
#include <QtMath>
#include <algorithm>

CommandSettings::CommandSettings(quint16 code, const QString& unit,
                                 double divider, quint8 tolerance,
                                 uint interval, bool isSigned,
                                 bool isTemperature)
    : m_code(code),
      m_unit(unit),
      m_divider(divider),
      m_tolerance(tolerance),
      m_isSigned(isSigned),
      m_isTemperature(isTemperature),
      m_interval(interval) {}

// static methods

const int DevCommand::maxLogValues = 100;
const QSet<quint16> DevCommand::logCommandsSet({6,  7,  8,  9,  10, 11, 12,
                                                13, 14, 15, 16, 17, 18, 19,
                                                20, 21, 22, 23, 24, 25, 26,
                                                27, 28, 29, 30, 31});

double DevCommand::convertCelToFar(double value) {
  return value * 9.0 / 5.0 + 32.0;
}

double DevCommand::convertFarToCel(double value) {
  return (value - 32.0) * 5.0 / 9.0;
}

// public methods
DevCommand::DevCommand(const CommandSettings& conf)
    : m_config(conf), m_tempId(AppSettings::getTemperatureUnit()) {
  m_rawValue = 0;
  m_strValue = QString::number(0, 'f', static_cast<int>(m_config.m_tolerance));
  if (logCommandsSet.contains(conf.m_code)) {
    m_logValues.reset(new QVector<double>(maxLogValues, 0));
    m_logValues->shrink_to_fit();
  }
}

void DevCommand::changeTemperatureUnit(Const::TemperatureUnitId id) {
  if (m_config.m_isTemperature && m_tempId != id) {
    m_tempId = id;
    switch (id) {
      default:
      case Const::TemperatureUnitId::kCelsius:
        m_value = convertFarToCel(m_value);
        break;
      case Const::TemperatureUnitId::kFahrenheit:
        m_value = convertCelToFar(m_value);
        break;
    }
    m_iValue = static_cast<int>(m_value);
    m_strValue =
        QString::number(m_value, 'f', static_cast<int>(m_config.m_tolerance));
    emit updatedValue();

    emit updatedUnit(unit());
  }
}

quint16 DevCommand::code() const { return m_config.m_code; }

QString DevCommand::unit() const {
  if (isTemperature()) {
    switch (m_tempId) {
      default:
      case Const::TemperatureUnitId::kCelsius:
        return m_config.m_unit + "C";
        break;
      case Const::TemperatureUnitId::kFahrenheit:
        return m_config.m_unit + "F";
        break;
    }
  } else {
    return m_config.m_unit;
  }
}

double DevCommand::divider() const { return m_config.m_divider; }

int DevCommand::tolerance() const {
  return static_cast<int>(m_config.m_tolerance);
}

bool DevCommand::isSigned() const { return m_config.m_isSigned; }

bool DevCommand::isTemperature() const { return m_config.m_isTemperature; }

double DevCommand::valueDouble() const { return m_value; }

uint DevCommand::valueInt() const { return m_iValue; }

QString DevCommand::valueStr() const { return m_strValue; }

uint DevCommand::interval() const { return m_config.m_interval; }

double DevCommand::avgValue() const {
  return m_cmdSum / static_cast<double>(maxLogValues);
}

double DevCommand::maxValue() const {
  return (m_logValues)
             ? *std::max_element(m_logValues->begin(), m_logValues->end())
             : 0;
}

double DevCommand::minValue() const {
  return (m_logValues)
             ? *std::min_element(m_logValues->begin(), m_logValues->end())
             : 0;
}

CommandSettings DevCommand::commandSettings() { return m_config; }

// public slots
void DevCommand::setFromDevice(quint16 value) {
  bool check = (value != m_rawValue);
  m_rawValue = value;

  double d = 0;
  if (isSigned()) {
    d = static_cast<double>(static_cast<int16_t>(m_rawValue));
    m_value =
        qRound(d / m_config.m_divider * qPow(10, m_config.m_tolerance) - 0.5) /
        qPow(10, m_config.m_tolerance);
  } else {
    d = static_cast<double>(static_cast<quint16>(m_rawValue));
    m_value =
        qRound(d / m_config.m_divider * qPow(10, m_config.m_tolerance) - 0.5) /
        qPow(10, m_config.m_tolerance);
  }

  if (m_config.m_isTemperature &&
      m_tempId == Const::TemperatureUnitId::kFahrenheit) {
    m_value = convertCelToFar(m_value);
  }

  if (m_logValues) {
    if (m_cmdIt < m_logValues->size()) {
      m_cmdSum -= m_logValues->at(m_cmdIt);
      (*m_logValues)[m_cmdIt++] = m_value;
      m_cmdSum += m_value;
    } else
      m_cmdIt = 0;
  }

  m_iValue = static_cast<int>(m_value);
  if (check || firstRun) {
    firstRun = false;
    m_strValue =
        QString::number(m_value, 'f', static_cast<int>(m_config.m_tolerance));

    emit updatedValue();
  }
}

void DevCommand::setFromWidget(int value) {
  emit sendValueSignal(m_config.m_code,
                       getRawFromValue(static_cast<double>(value)));
}

void DevCommand::setFromWidget(double value) {
  emit sendValueSignal(m_config.m_code, getRawFromValue(value));
}

quint16 DevCommand::getRawFromValue(double value) const {
  if (m_config.m_isTemperature &&
      m_tempId == Const::TemperatureUnitId::kFahrenheit) {
    value = convertFarToCel(value);
  }

  return static_cast<quint16>(qRound(value * m_config.m_divider - 0.5));
}
