#ifndef DEVICEFOLDEDWIDGET_H
#define DEVICEFOLDEDWIDGET_H

#include <model/event.h>

#include <QWidget>

#include "group/GroupInterface.h"
#include "widgets/warningwidget.h"
namespace Ui {
class DeviceFoldedWidget;
}

class DeviceFoldedWidget : public QWidget, public GuiWidgetInterface {
  Q_OBJECT

 public:
  explicit DeviceFoldedWidget(QWidget *parent = nullptr);
  ~DeviceFoldedWidget();
  void updateValue(const model::Event &event);
  void updateStyle() override;
 signals:
  void showWidget();
 public slots:
  void setStatus(DeviceStatusGroup &status);

 private:
  Ui::DeviceFoldedWidget *ui;
  WarningWidget *m_warning;
  QSet<GuiWidgetBase *> m_widgetsTable;
};

#endif  // DEVICEFOLDEDWIDGET_H
