#ifndef DEVICEFOLDEDWIDGET_H
#define DEVICEFOLDEDWIDGET_H

#include <model/event.h>

#include <QWidget>

#include "group/GroupInterface.h"
#include "model/device/DeviceWidgetDesc.h"
#include "widgets/warningwidget.h"
class FoldedControlWidget;
class ButtonWidget;

namespace Ui {
class DeviceFoldedWidget;
}

class DeviceFoldedWidget : public QWidget, public GuiWidgetInterface {
  Q_OBJECT

 public:
  explicit DeviceFoldedWidget(
      const DeviceWidgetDesc &description,
      const QMap<quint16, QSharedPointer<CommandConverter>> &converters,
      QWidget *parent = nullptr);
  ~DeviceFoldedWidget();
  void setAddress(int addr);
  void updateValue(const model::Event &event);
  void updateStyle() override;
  void setPinnedWidgets(QMap<QString, bool> &widgets);
 signals:
  void showWidget();
  void acceptDataFromWidget(quint16 code, quint16 value);
 public slots:
  void setLink(bool link);
  void setStatus(DeviceStatusGroup &status);

 private:
  Ui::DeviceFoldedWidget *ui;
  const QMap<quint16, Button> &m_buttons;
  QMap<quint16, QSharedPointer<CommandConverter>> m_converters;
  QSet<FoldedControlWidget *> m_folded;
  QHBoxLayout *m_widgetLayout;
  WarningWidget *m_warning;
  QSet<GuiWidgetBase *> m_widgetsTable;
  ButtonWidget *m_laserButton{nullptr};
  ButtonWidget *m_tecButton{nullptr};
};

#endif  // DEVICEFOLDEDWIDGET_H
