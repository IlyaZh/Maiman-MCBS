#ifndef MODELCOMMANDS_H
#define MODELCOMMANDS_H

#include "ModelCommandInterface.h"
#include "ModelInterface.h"
#include "enums.h"

class ConnectModelCommand : public ModelCommand {
public:
    ConnectModelCommand(class ModelInterface* model) : m_model(model) {}
    bool execute() const override;
    CONNECT_PROTOCOL protocol;
    QString host;
    int port;
private:
    class ModelInterface* m_model;
};

#endif // MODELCOMMANDS_H
