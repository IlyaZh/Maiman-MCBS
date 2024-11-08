#ifndef DEVICECONDITION_H
#define DEVICECONDITION_H

#include <QLabel>
#include <QObject>
#include <QWidget>

#include "gui/guiinterface.h"
#include "model/device/devicewidget.h"

class CommandConverter;

class DeviceCondition : public GuiWidgetBase, public GuiWidgetInterface {
 public:
  explicit DeviceCondition(
      const QMap<quint16, QSharedPointer<CommandConverter>>& converters,
      const QVector<Led>& Leds, QLabel* label, QWidget* parent = nullptr);
  ~DeviceCondition() override;
  void setData(quint16 code, quint16 data) override;
  QVector<quint16> Subscribe() override;
  void updateStyle() override;
 signals:

 private slots:
  void setStateText(uint value, LedMask led);

 private:
  const QMap<quint16, QSharedPointer<CommandConverter>>& m_converters;
  QVector<Led> m_Leds;
  QPointer<QLabel> m_Label;
  QVector<quint16> m_codes;
  QStringList m_states;
};

#endif  // DEVICECONDITION_H
