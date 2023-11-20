#pragma once

#include <QObject>
#include <QVector>
#include <QWidget>

#include "constants.h"

class CommandSettings;

class CommandConverter {
 public:
  CommandConverter(const CommandSettings& conf);
  ~CommandConverter();
  void setValue(quint16 value);
  double convertCelToFar();
  double convertFarToCel();
  quint16 code() const;
  QString unit() const;
  double divider() const;
  int tolerance() const;
  bool isSigned() const;
  bool isTemperature() const;
  double valueDouble() const;
  uint valueInt() const;
  QString valueStr() const;
  uint interval() const;
  quint16 getRawFromValue(double value) const;

 private:
  const CommandSettings& m_config;
  quint16 m_rawValue = 0;
  double m_value = 0;
};
