#include "groupcommandfactory.h"

GroupCommandFactory::GroupCommandFactory() {}

const model::Event GroupCommandFactory::createGroupCommand(
    const QSet<quint8>& addresses,
    const model::events::network::CommandType& command) {
  model::Event event(model::EventType::kWriteDevice,
                     model::events::network::FrontRequest(addresses, command));
  return event;
}
