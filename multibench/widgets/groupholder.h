#ifndef GROUPHOLDER_H
#define GROUPHOLDER_H

#include <QVBoxLayout>
#include <QWidget>

#include "widgets/groupwidget.h"
#include "widgets/groupwidgetfolded.h"

class GroupWidgetFolded;
class GroupWidget;

class GroupHolder : public QWidget, public GuiWidgetInterface {
  Q_OBJECT
 public:
  explicit GroupHolder(int groupAddr, QWidget *parent = nullptr);
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
  QSet<quint8> getAddresses() const;
 private slots:
  void startDevices();
  void stopDevices();
  void hideGroup(bool flag);
  void showGroup(bool flag);
 signals:
  void groupEvent(model::Event);
  void nameEdited(QString name, int addr);
  void statusChanged(QMap<quint8, DeviceStatusGroup> &status);
  void linkChanged(int addr, bool status);
  void closeGroupStatusDialog();

 private:
  QStackedLayout *m_stacked;
  QPointer<GroupWidget> m_expandedWidget;
  QPointer<GroupWidgetFolded> m_foldedWidget;
  QList<QPointer<DeviceHolder>> m_groupWidgets;
  QMap<quint8, QSharedPointer<DeviceStatusGroup>> m_status;
  QSet<quint8> m_addresses;
  QMap<quint8, bool> m_linked;
  QString m_name;
  bool m_hideDevices = false;
  bool m_allStarted = false;
  int m_selfAddr = 0;
};

#endif  // GROUPHOLDER_H
