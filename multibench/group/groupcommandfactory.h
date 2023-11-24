#pragma once

#include <QObject>

#include "model/event.h"

using namespace model;

class GroupCommandFactory {
 public:
  GroupCommandFactory();
  static Event createGroupCommand(QSet<quint8>& addresses,
                                  events::network::CommandType& command);
};
