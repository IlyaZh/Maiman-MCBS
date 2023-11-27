#pragma once

#include <QWidget>

class GroupInterface {
 public:
  GroupInterface() = default;
  virtual ~GroupInterface() = default;
  virtual void addGroupMember(QSharedPointer<GroupInterface> member) = 0;
  virtual void removeGroupMember(QSharedPointer<GroupInterface> member) = 0;
};
