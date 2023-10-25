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

class ControlWidget : public QWidget, public GuiInterface {
  Q_OBJECT

 public:
  explicit ControlWidget(QStringView name, QSharedPointer<DevCommand> Value,
                         QSharedPointer<DevCommand> Max,
                         QSharedPointer<DevCommand> Min,
                         QSharedPointer<DevCommand> Real,
                         QWidget *parent = nullptr);
  ~ControlWidget() override;
  void getData(QSharedPointer<CommandConverter> data) override;
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
  bool isUserEdit = false;

  void adjust();
 private slots:
  void userEnteredValue();
};

#endif  // CONTROLWIDGET_H
