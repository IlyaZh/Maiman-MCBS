#pragma once

#include <QObject>

#include "model/event.h"

class GroupCommandFactory {
 public:
  GroupCommandFactory();
  const static model::Event createGroupCommand(
      const QSet<quint8>& addresses,
      const model::events::network::CommandType& command);
};
