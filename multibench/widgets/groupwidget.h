#pragma once

#include <QWidget>

#include "group/GroupInterface.h"

namespace Ui {
class GroupWidget;
}

class GroupWidget : public QWidget, public GroupInterface {
  Q_OBJECT

 public:
  explicit GroupWidget(QWidget *parent = nullptr);
  ~GroupWidget() override;
  void addGroupMember(QSharedPointer<GroupInterface> member) override;
  void removeGroupMember(QSharedPointer<GroupInterface> member) override;
 private slots:

 private:
  Ui::GroupWidget *ui;
};
