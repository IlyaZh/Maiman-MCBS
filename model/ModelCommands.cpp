#include "ModelCommands.h"

bool ConnectModelCommand::execute() const {
    if(host.isEmpty() || port == 0 || protocol == ModelInterface::UNKNOWN_PROTOCOL) {
        return false;
    }

    m_model->start(protocol, host, port);
    return true;

}

