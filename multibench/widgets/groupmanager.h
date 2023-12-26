#pragma once

#include <QDialog>
#include <QtWidgets>

namespace Ui {
class GroupManager;
}

class GroupManager : public QDialog {
  Q_OBJECT

 public:
  explicit GroupManager(QSet<quint8> addresses, QWidget* parent = nullptr);
  ~GroupManager();
 private slots:
  void createGroup();
  void deleteGroup();
 signals:
  void createGroupWidget(QSet<quint8> addrs);
  void deleteGroupWidget(QSet<quint8> addrs);

 private:
  Ui::GroupManager* ui;
  QGridLayout* m_devicesFieldLayout;
  QGridLayout* m_groupsFieldLayout;
  QSet<quint8> m_addresses;
  QMap<quint8, QCheckBox*> m_checkBoxes;
  QMap<QCheckBox*, QSet<quint8>> m_groupCheckBoxes;
};
