#include "groupcommandfactory.h"

GroupCommandFactory::GroupCommandFactory() {}

const model::Event GroupCommandFactory::createGroupCommand(
    QSet<quint8>& addresses, model::events::network::CommandType& command) {
  model::Event event(model::EventType::kWriteDevice,
                     model::events::network::FrontRequest(addresses, command));
  return event;
}
