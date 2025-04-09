#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QtWidgets>

#include "commandconverter.h"
#include "group/GroupInterface.h"
#include "gui/DeviceSmallWidgets.h"
#include "gui/guiinterface.h"
#include "gui/guimediator.h"
#include "model/device/DeviceWidgetDesc.h"
#include "widgets/buttonwidget.h"
#include "widgets/inlineedit.h"

class DevCommand;
class ControlWidget;
class BinaryWidget;
class HiddenWidget;
class DeviceCondition;
class ButtonWidget;
class CommandConverter;

namespace Ui {
class DeviceWidget;
}

class DeviceWidget : public QWidget,
                     public GroupInterface,
                     public GuiWidgetInterface {
  Q_OBJECT
 public:
  explicit DeviceWidget(
      const DeviceWidgetDesc& description,
      const QMap<quint16, QSharedPointer<CommandConverter>>& converters,
      QWidget* parent = nullptr);
  ~DeviceWidget() override;
  void setAddress(int addr);
  QString getName() const;
  void setConstraint(bool state);
  void updateValue(const model::Event& event);
  void addGroupMember(QSharedPointer<GroupInterface> member) override;
  void removeGroupMember(QSharedPointer<GroupInterface> member) override;
  void updateStyle() override;

 public slots:
  void setLink(bool link);
 signals:
  void nameEdited(QString name, int addr);
  void dataIncome(QSharedPointer<CommandConverter> command);
  void acceptDataFromWidget(quint16 code, quint16 value);
  void hideWidget(QMap<QString, bool>& widgets);

 private:
  Ui::DeviceWidget* ui;
  const QMap<quint16, Button>& m_buttons;
  QVector<HiddenWidget*> m_widgets;
  QMap<int, QString> m_pinnedWidgets;
  QMap<quint16, QSharedPointer<CommandConverter>> m_converters;
  QGridLayout* m_widgetLayout;
  QMap<quint16, BinaryWidget*> m_binaryWidgets;
  QVector<QPushButton*> m_pinButtons;
  bool m_hideControls{false};
  ButtonWidget* m_laserButton{nullptr};
  ButtonWidget* m_tecButton{nullptr};
  DeviceCondition* m_deviceCondition;
  InLineEdit* m_deviceAddress;
  int m_fixedWidgets{0};
  QSet<GuiWidgetBase*> m_widgetsTable;
  void paintEvent(QPaintEvent*) override;
  void adjust();

 private slots:
  void hideControlsButtonClicked(bool flag);
  void pinButtonClicked(int idx, bool state);
};
#endif  // DEVICEWIDGET_H
