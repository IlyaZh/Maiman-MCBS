#pragma once

#include <QObject>

#include "model/event.h"

class GroupCommandFactory {
 public:
  GroupCommandFactory();
  const static model::Event createGroupCommand(
      QSet<quint8>& addresses, model::events::network::CommandType& command);
};
