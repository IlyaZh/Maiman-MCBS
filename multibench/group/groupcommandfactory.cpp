#include "groupcommandfactory.h"

GroupCommandFactory::GroupCommandFactory() {}

Event GroupCommandFactory::createGroupCommand(
    QSet<quint8>& addresses, events::network::CommandType& command) {
  Event event(EventType::kWriteDevice,
              events::network::FrontRequest(addresses, command));
  return event;
}
