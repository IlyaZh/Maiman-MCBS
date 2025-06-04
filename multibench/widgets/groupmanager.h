#pragma once

#include <QDialog>
#include <QtWidgets>

#include "constants.h"
#include "gui/guiinterface.h"
#include "widgets/groupholder.h"

struct deviceCheckBox {
  int d_addr;
  QCheckBox* d_checkBox;
  bool d_isDeleted = false;
};

struct groupCheckBoxes {
  int g_addr;
  QCheckBox* g_checkBox;
  QMap<quint8, QSharedPointer<deviceCheckBox>> g_subBoxes;
  QVBoxLayout* g_layout;
  bool g_isDeleted = false;
};

namespace Ui {
class GroupManager;
}

class GroupManager : public QDialog, public GuiWidgetInterface {
  Q_OBJECT

 public:
  explicit GroupManager(const QMap<quint8, QPointer<DeviceHolder>>& devices,
                        const QMap<int, QPointer<GroupHolder>>& groups,
                        QWidget* parent = nullptr);
  ~GroupManager();
  void updateStyle() override;
 private slots:
  void addDeviceToGroup();
  void removeDeviceFromGroup();
  void groupButtonClicked(QAbstractButton* button);
  void createOneGroup(int g_addr, const QSet<quint8> devicesAddrs,
                      const QString& name = "");
  void checkBoxClicked(bool status, int addr);
 signals:
  void sendAllGroups(const QMap<int, QSharedPointer<groupCheckBoxes>>& groups);

 private:
  void sortWidgets();
  Ui::GroupManager* ui;
  QVBoxLayout* m_devicesFieldLayout;
  QVBoxLayout* m_groupsFieldLayout;
  QMap<quint8, QSharedPointer<deviceCheckBox>> m_devicesContainer;
  QMap<int, QSharedPointer<groupCheckBoxes>> m_groupsContainer;
  const QMap<quint8, QPointer<DeviceHolder>>& m_devices;
  const QMap<int, QPointer<GroupHolder>>& m_groups;
  QButtonGroup* m_buttonGroup;
  QFont m_font;
};
