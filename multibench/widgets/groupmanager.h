#pragma once

#include <QDialog>
#include <QtWidgets>

#include "widgets/groupwidget.h"

struct groupsCheckBoxes {
  QCheckBox* groupBox_;
  QMap<quint8, QCheckBox*> subBoxes_;
};

namespace Ui {
class GroupManager;
}

class GroupManager : public QDialog {
  Q_OBJECT

 public:
  explicit GroupManager(QMap<quint8, QString> addresses,
                        QVector<QPointer<GroupWidget>>& groups,
                        QWidget* parent = nullptr);
  ~GroupManager();
 public slots:
  void finishGroupAction();
 private slots:
  void createGroup();
  void deleteGroup();
 signals:
  void createGroupWidget(QSet<quint8> addrs, int groupAddr);
  void deleteGroupWidget(QSet<quint8> addrs);

 private:
  Ui::GroupManager* ui;
  QGridLayout* m_devicesFieldLayout;
  QGridLayout* m_groupsFieldLayout;
  QMap<quint8, QString> m_addresses;
  QMap<quint8, QCheckBox*> m_checkBoxes;
  QMap<int, groupsCheckBoxes> m_groupsTable;
  QVector<QPointer<GroupWidget>>& m_groups;
  QFont m_font;
  void paintDevices();
};
