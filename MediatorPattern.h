#ifndef MEDIATORPATTERN_H
#define MEDIATORPATTERN_H

#include <QString>
#include <QVariant>

class BaseComponent;

class Mediator {
public:
    virtual void Notify(BaseComponent *sender, QString event, QVariant value) const = 0;
};

class BaseComponent {
public:
    BaseComponent(Mediator* mediator = nullptr) : m_mediator(mediator)
    {

    }
    void setMediator(Mediator* mediator) {
        this->m_mediator = mediator;
    }
protected:
    Mediator *m_mediator = nullptr;
};


#endif // MEDIATORPATTERN_H
