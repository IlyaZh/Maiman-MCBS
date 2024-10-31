#pragma once

#include <QDialog>
#include <QtWidgets>

#include "constants.h"
#include "gui/guiinterface.h"
#include "widgets/groupwidget.h"

struct groupsCheckBoxes {
  int groupAddr_;
  QCheckBox* groupBox_;
  QMap<quint8, QCheckBox*> subBoxes_;
  QVBoxLayout* layout_;
};

namespace Ui {
class GroupManager;
}

class GroupManager : public QDialog, public GuiWidgetInterface {
  Q_OBJECT

 public:
  explicit GroupManager(const QMap<quint8, QPointer<DeviceWidget>>& devices,
                        const QMap<int, QPointer<GroupWidget>>& groups,
                        QWidget* parent = nullptr);
  ~GroupManager();
  void updateStyle() override;
 public slots:
  void finishGroupAction();
 private slots:
  void createGroup();
  void deleteGroup();
  void groupButtonClicked(QAbstractButton* button);
 signals:
  void createGroupWidget(QSet<quint8> addrs, int groupAddr);
  void deleteGroupWidget(int addr);
  void removeMemberGroup(int groupAdd, quint8 devAddr);
  void addMemberGroup(int groupAdd, quint8 devAddr);

 private:
  Ui::GroupManager* ui;
  QVBoxLayout* m_devicesFieldLayout;
  QVBoxLayout* m_groupsFieldLayout;
  QMap<quint8, QCheckBox*> m_checkBoxes;
  QMap<int, QSharedPointer<groupsCheckBoxes>> m_groupsTable;
  const QMap<quint8, QPointer<DeviceWidget>>& m_devices;
  const QMap<int, QPointer<GroupWidget>>& m_groups;
  QButtonGroup* m_buttonGroup;
  QFont m_font;
  void paintDevices();
  void clearGroup(QSharedPointer<groupsCheckBoxes> group);
};
