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
  explicit GroupWidget(int groupAddr, QWidget *parent = nullptr);
  ~GroupWidget() override;
  void addGroupMember(QPointer<DeviceHolder> member);
  void removeGroupMember(QPointer<DeviceHolder> member);
  const QSet<quint8> getAddresses();
  void setDevicesStatus(quint8 addr, QSharedPointer<DeviceStatusGroup> desc);
  const QString getName();
  void setName(QString name);
  int getGroupAddress();
  void linkStatusChanged(int addr, bool status);
  void updateValue(const model::Event &event);
  void updateStyle() override;
 signals:
  void groupEvent(model::Event);
  void nameEdited(QString name, int addr);
  void statusChanged(QMap<quint8, DeviceStatusGroup> &status);
  void linkChanged(int addr, bool status);
  void closeGroupStatusDialog();
 private slots:
  void startDevices();
  void stopDevices();
  void hideDevices(bool flag);
  void showStatus();

 private:
  void resizeWidget();
  void paintEvent(QPaintEvent *) override;
  Ui::GroupWidget *ui;
  QPushButton *m_hideButton;
  QPushButton *m_statusButton;
  QGridLayout *m_widgetLayout;
  QList<QPointer<DeviceHolder>> m_groupWidgets;
  QMap<quint8, QSharedPointer<DeviceStatusGroup>> m_status;
  QSet<quint8> m_addresses;
  QMap<quint8, bool> m_linked;
  InLineEdit *m_name;
  bool m_hideDevices = false;
  bool m_allStarted = false;
  int m_selfAddr = 0;
  WarningWidget *m_warning;
};
