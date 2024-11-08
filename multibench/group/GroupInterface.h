#pragma once

#include <QWidget>

class GroupInterface {
 public:
  virtual ~GroupInterface() = default;
  virtual void addGroupMember(QSharedPointer<GroupInterface> member) = 0;
  virtual void removeGroupMember(QSharedPointer<GroupInterface> member) = 0;
};
