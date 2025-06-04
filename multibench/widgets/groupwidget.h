#pragma once

#include <QtWidgets>

#include "group/GroupInterface.h"
#include "gui/DeviceSmallWidgets.h"
#include "gui/guiinterface.h"
#include "model/device/deviceholder.h"
#include "widgets/groupstatusdialog.h"
#include "widgets/inlineedit.h"
#include "widgets/warningwidget.h"

class GroupInterface;
namespace Ui {
class GroupWidget;
}

class GroupWidget : public QWidget, public GuiWidgetInterface {
  Q_OBJECT
 public:
  explicit GroupWidget(QWidget *parent = nullptr);
  ~GroupWidget() override;
  void addGroupMember(QPointer<DeviceHolder> member);
  void removeGroupMember(QPointer<DeviceHolder> member);
  void setName(QString name);
  void linkStatusChanged(bool status);
  void updateStyle() override;
  void addDevicesData(QMap<quint8, QSharedPointer<DeviceStatusGroup>> &status);
 signals:
  void closeGroupStatusDialog();
  void sizeChanged();
  void startAll(bool checked);
  void stopAll(bool checked);
  bool hideWidget(bool flag);
 private slots:
  void hideDevices(bool flag);
  void showStatus();
  void isHiddenWidget(bool state);

 private:
  void resizeWidget();
  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *event) override;
  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;
  bool findHiddenDevices();
  Ui::GroupWidget *ui;
  QPushButton *m_hideButton;
  QPushButton *m_statusButton;
  QGridLayout *m_widgetLayout;
  QList<QPointer<DeviceHolder>> m_groupWidgets;
  //  InLineEdit *m_name;
  bool m_hideDevices = false;
  WarningWidget *m_warning;
};
