#ifndef DEVICECONDITION_H
#define DEVICECONDITION_H

#include <QLabel>
#include <QObject>
#include <QWidget>

#include "model/device/devicewidget.h"

class DevCommand;

class DeviceCondition : public QObject {
  Q_OBJECT
 public:
  explicit DeviceCondition(
      const QMap<quint16, QSharedPointer<DevCommand>>& commands,
      const QVector<Led>& Leds, QLabel* label, QObject* parent = nullptr);

 signals:

 private slots:
  void setStateText(uint value, LedMask led);

 private:
  const QMap<quint16, QSharedPointer<DevCommand>>& m_commands;
  QVector<Led> m_Leds;
  QScopedPointer<QLabel> m_Label;
  QStringList m_states;
};

#endif  // DEVICECONDITION_H
