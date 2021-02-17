#include "ModelCommands.h"
#include "enums.h"

bool ConnectModelCommand::execute() const {
    if(host.isEmpty() || port == 0 || protocol == UNKNOWN_PROTOCOL) {
        return false;
    }

    m_model->start(protocol, host, port);
    return true;

}

