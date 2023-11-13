#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QLabel>
#include <QMap>
#include <QValidator>
#include <QWidget>

#include "device/commandsettings.h"
#include "gui/guiinterface.h"

struct Control;

namespace Ui {
class CommandWidget;
}  // namespace Ui

class ControlWidget : public GuiWidgetBase {
 public:
  explicit ControlWidget(QStringView name, QSharedPointer<DevCommand> Value,
                         QSharedPointer<DevCommand> Max,
                         QSharedPointer<DevCommand> Min,
                         QSharedPointer<DevCommand> Real,
                         QWidget *parent = nullptr);
  ControlWidget(QStringView name, QSharedPointer<CommandConverter> Value,
                QSharedPointer<CommandConverter> Max,
                QSharedPointer<CommandConverter> Min,
                QSharedPointer<CommandConverter> Real,
                QWidget *parent = nullptr);
  ~ControlWidget() override;
  void getData(quint16 code, quint16 data) override;
  QVector<quint16> Subscribe() override;

 private slots:
  void setValue();
  void setMaxValue();
  void setMinValue();
  void setRealValue();
  void setUnits(QStringView units);
  void setEditLineRed();
  void setEditLineWhite();

 private:
  Ui::CommandWidget *ui;
  QDoubleValidator *m_Validator;
  QVector<DevCommand> m_Commands;
  QSharedPointer<DevCommand> m_Real;
  QSharedPointer<DevCommand> m_Value;
  QSharedPointer<DevCommand> m_Max;
  QSharedPointer<DevCommand> m_Min;
  QSharedPointer<CommandConverter> m_RealConv;
  QSharedPointer<CommandConverter> m_ValueConv;
  QSharedPointer<CommandConverter> m_MaxConv;
  QSharedPointer<CommandConverter> m_MinConv;
  bool isUserEdit = false;
  QVector<quint16> m_codes;

  void adjust();
 private slots:
  void userEnteredValue();
  void userEnteredData();
};

#endif  // CONTROLWIDGET_H
