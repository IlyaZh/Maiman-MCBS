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

class HiddenWidget;
class DeviceCondition;
class ButtonWidget;

namespace Ui {
class DeviceWidget;
}

class DeviceWidget : public QWidget, public GuiWidgetInterface {
  Q_OBJECT
 public:
  explicit DeviceWidget(const QVector<HiddenWidget*>& widgets,
                        DeviceCondition* deviceCondition,
                        QWidget* parent = nullptr);
  ~DeviceWidget() override;
  void setAddress(int addr);
  QString getName() const;
  void setModel(QString model);
  void setConstraint(bool state);
  void updateStyle() override;
  void setButton(QPointer<ButtonWidget> pButton);
 public slots:
  void setLink(bool link);
 signals:
  void nameEdited(QString name, int addr);
  void dataIncome(QSharedPointer<CommandConverter> command);
  void acceptDataFromWidget(quint16 code, quint16 value);
  void hideWidget();

 private:
  Ui::DeviceWidget* ui;
  QVector<HiddenWidget*> m_widgets;
  QGridLayout* m_widgetLayout;
  QVector<QPushButton*> m_pinButtons;
  InLineEdit* m_deviceAddress;
  int m_fixedWidgets{0};
  int m_fixedEndWidgets{0};
  void paintEvent(QPaintEvent*) override;
  void adjust();

 private slots:
  void pinButtonClicked(int idx, bool state);
};
#endif  // DEVICEWIDGET_H
