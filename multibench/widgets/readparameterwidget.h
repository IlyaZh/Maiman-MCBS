#ifndef READPARAMETERWIDGET_H
#define READPARAMETERWIDGET_H

#include <QMap>
#include <QWidget>
// #include <QTest>
#include <QLabel>
#include <QLayout>
#include <QSharedPointer>
#include <QSpacerItem>

#include "gui/guiinterface.h"

struct Control;
class DevCommand;

class ReadParameterWidget : public GuiWidgetBase {
 public:
  ReadParameterWidget();
  virtual ~ReadParameterWidget() = default;

  virtual void setup(QStringView name, QSharedPointer<DevCommand> cmd);
  virtual void setup(QStringView name, QSharedPointer<CommandConverter> cmd);
  int getUnitslength();
  void setUnitsLength(int length);
  void setData(quint16 code, quint16 data) override;
  QVector<quint16> Subscribe() override;
 signals:
  void setDataFromWidget(quint16 code, quint16 data);

 protected:
  QString m_unit;
  QSharedPointer<DevCommand> m_command;
  QSharedPointer<CommandConverter> m_converter;
  QHBoxLayout* m_layout{nullptr};
  QLabel* m_labelParameter{nullptr};
  QLabel* m_labelValue{nullptr};
  QLabel* m_labelUnit{nullptr};
  QVector<quint16> m_codes;

  void setValue(double value, int decimal);
  void setValue(int value);
  void setUnit(QStringView unit);
};

#endif  // READPARAMETERWIDGET_H
