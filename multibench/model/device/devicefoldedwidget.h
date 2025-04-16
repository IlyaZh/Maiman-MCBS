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
      const QVector<FoldedControlWidget *> &foldedWidgets,
      QWidget *parent = nullptr);
  ~DeviceFoldedWidget();
  void setAddress(int addr);
  void updateStyle() override;
  void setButton(QPointer<ButtonWidget> pButton);
  void setVisibleWidget();
  void setModel(QString model);
 signals:
  void showWidget();
  void acceptDataFromWidget(quint16 code, quint16 value);
 public slots:
  void setLink(bool link);
  void setStatus(DeviceStatusGroup &status);

 private:
  Ui::DeviceFoldedWidget *ui;
  QHBoxLayout *m_widgetLayout;
  const QVector<FoldedControlWidget *> &m_foldedWidgets;
  WarningWidget *m_warning;
};

#endif  // DEVICEFOLDEDWIDGET_H
