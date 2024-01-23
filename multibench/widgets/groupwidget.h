#pragma once

#include <QtWidgets>

#include "group/GroupInterface.h"
#include "model/device/devicewidget.h"

struct DeviceStatusGroup {
  std::optional<QStringList> errors;
  std::optional<QMap<QString, bool>> devStarted;
};

class GroupInterface;
namespace Ui {
class GroupWidget;
}

class GroupWidget : public QWidget {
  Q_OBJECT
 public:
  explicit GroupWidget(QWidget *parent = nullptr);
  ~GroupWidget() override;
  void addGroupMember(QPointer<DeviceWidget> member);
  void removeGroupMember(QPointer<DeviceWidget> member);
  const QSet<quint8> getAddresses();
  void setDevicesStatus(quint8 addr, QSharedPointer<DeviceStatusGroup> desc);
 signals:
  void groupEvent(model::Event);
  void nameEdited(QString name, int addr);
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
};
