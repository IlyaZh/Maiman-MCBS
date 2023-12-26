#pragma once

#include <QtWidgets>

#include "group/GroupInterface.h"
#include "model/device/devicewidget.h"
class GroupInterface;
namespace Ui {
class GroupWidget;
}

class GroupWidget : public QWidget {
  Q_OBJECT
 public:
  explicit GroupWidget(QWidget *parent = nullptr);
  ~GroupWidget();
  void addGroupMember(QPointer<DeviceWidget> member);
  void removeGroupMember(QPointer<DeviceWidget> member);
  const QSet<quint8> getAddresses();
 signals:
  void groupEvent(model::Event);
 private slots:
  void startDevices();
  void stopDevices();

 private:
  void resizeWidget();
  Ui::GroupWidget *ui;
  QGridLayout *m_widgetLayout;
  QList<QPointer<DeviceWidget>> m_groupWidgets;
  QSet<quint8> m_addresses;
};
