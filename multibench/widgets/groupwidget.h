#pragma once

#include <QtWidgets>

#include "group/GroupInterface.h"
#include "model/device/devicewidget.h"
#include "widgets/groupstatusdialog.h"

//struct DeviceStatusGroup {
//  std::optional<QStringList> errors;
//  std::optional<QMap<QString, bool>> devStarted;
//};

class GroupInterface;
namespace Ui {
class GroupWidget;
}

class GroupWidget : public QWidget {
  Q_OBJECT
 public:
  explicit GroupWidget(int groupAddr, QWidget *parent = nullptr);
  ~GroupWidget() override;
  void addGroupMember(QPointer<DeviceWidget> member);
  void removeGroupMember(QPointer<DeviceWidget> member);
  const QSet<quint8> getAddresses();
  void setDevicesStatus(quint8 addr, QSharedPointer<DeviceStatusGroup> desc);
  const QString getName();
  int getGroupAddress();
  void linkStatusChanged(int addr, bool status);
 signals:
  void groupEvent(model::Event);
  void nameEdited(QString name, int addr);
  void statusChanged(QMap<quint8, DeviceStatusGroup>& status);
  void linkChanged(int addr, bool status);
 private slots:
  void startDevices();
  void stopDevices();
  void hideDevices(bool flag);
  void showStatus();

 private:
  void resizeWidget();
  void paintEvent(QPaintEvent *) override;
  Ui::GroupWidget *ui;
  QGridLayout *m_widgetLayout;
  QList<QPointer<DeviceWidget>> m_groupWidgets;
  QMap<quint8, DeviceStatusGroup> m_status;
  QSet<quint8> m_addresses;
  bool m_hideDevices = false;
  QString m_name{};
  int m_selfAddr = 0;
};
