#ifndef MODELCOMMANDS_H
#define MODELCOMMANDS_H

#include "ModelInterface.h"

class ModelCommand {
public:
    virtual ~ModelCommand() {}
    virtual bool execute() const = 0;
};

class ConnectModelCommand : public ModelCommand {
public:
    ConnectModelCommand(class ModelInterface* model) : m_model(model) {}
    bool execute() const override;
    int protocol;
    QString host;
    int port;
private:
    class ModelInterface* m_model;
};

#endif // MODELCOMMANDS_H
