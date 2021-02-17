#ifndef MODELCOMMANDINTERFACE_H
#define MODELCOMMANDINTERFACE_H

class ModelCommand {
public:
    virtual ~ModelCommand() {}
    virtual bool execute() const = 0;
};


#endif // MODELCOMMANDINTERFACE_H
