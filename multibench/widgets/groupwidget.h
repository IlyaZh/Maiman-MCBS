#pragma once

#include <QtWidgets>

#include "group/GroupInterface.h"
#include "model/device/devicewidget.h"

struct DeviceStatusGroup {
  DeviceStatusGroup(const bool connected, const QString &id,
                    const QString &desc, const QString &errors)
      : m_connected(connected),
        m_id(id),
        m_description(desc),
        m_errors(errors) {}
  const bool m_connected;
  const QString m_id;
  const QString m_description;
  const QString m_errors;
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
 signals:
  void groupEvent(model::Event);
 private slots:
  void startDevices();
  void stopDevices();
  void hideDevices(bool flag);

 private:
  void resizeWidget();
  void paintEvent(QPaintEvent *) override;
  Ui::GroupWidget *ui;
  QGridLayout *m_widgetLayout;
  QList<QPointer<DeviceWidget>> m_groupWidgets;
  QSet<quint8> m_addresses;
  bool m_hideDevices = false;
};
