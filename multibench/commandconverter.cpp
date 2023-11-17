#include "commandconverter.h"

#include <QDebug>

#include "device/device.h"

CommandConverter::CommandConverter(const CommandSettings& conf)
    : m_config(conf), m_tempId(AppSettings::getTemperatureUnit()) {}
CommandConverter::~CommandConverter() {}

void CommandConverter::setValue(quint16 value) {
  double d = 0;
  if (m_config.m_isSigned) {
    d = static_cast<double>(static_cast<int16_t>(value));
    m_value =
        qRound(d / m_config.m_divider * qPow(10, m_config.m_tolerance) - 0.5) /
        qPow(10, m_config.m_tolerance);
  } else {
    d = static_cast<double>(static_cast<quint16>(value));
    m_value =
        qRound(d / m_config.m_divider * qPow(10, m_config.m_tolerance) - 0.5) /
        qPow(10, m_config.m_tolerance);
  }
  if (m_config.m_isTemperature and
      m_tempId == Const::TemperatureUnitId::kFahrenheit) {
    m_value = convertCelToFar();
  }
}

QString CommandConverter::unit() const {
  if (m_config.m_isTemperature) {
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

void CommandConverter::changeTemperatureUnit(Const::TemperatureUnitId id) {
  if (m_config.m_isTemperature) {
    m_tempId = id;
  }
}

double CommandConverter::convertCelToFar() {
  return m_value * 9.0 / 5.0 + 32.0;
}

double CommandConverter::convertFarToCel() {
  return (m_value - 32.0) * 5.0 / 9.0;
}

double CommandConverter::divider() const { return m_config.m_divider; }

int CommandConverter::tolerance() const {
  return static_cast<int>(m_config.m_tolerance);
}

bool CommandConverter::isSigned() const { return m_config.m_isSigned; }

bool CommandConverter::isTemperature() const {
  return m_config.m_isTemperature;
}

uint CommandConverter::interval() const { return m_config.m_interval; }

double CommandConverter::valueDouble() const { return m_value; }

uint CommandConverter::valueInt() const { return static_cast<int>(m_value); }

QString CommandConverter::valueStr() const {
  return QString::number(m_value, 'f', static_cast<int>(m_config.m_tolerance));
}

quint16 CommandConverter::code() const { return m_config.m_code; }

quint16 CommandConverter::getRawFromValue(double value) const {
  if (m_config.m_isTemperature &&
      m_tempId == Const::TemperatureUnitId::kFahrenheit) {
    value = (value - 32.0) * 5.0 / 9.0;
  }

  return static_cast<quint16>(qRound(value * m_config.m_divider - 0.5));
}
