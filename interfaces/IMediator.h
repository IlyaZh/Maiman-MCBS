#ifndef IMEDIATOR_H
#define IMEDIATOR_H

#include <QString>
#include <QVariant>

class IMediator;

class IMediatorBase {
public:
    IMediatorBase(IMediator* mediator = nullptr) : m_mediator(mediator) {

    }
    void setMediator(IMediator* mediator) { m_mediator = mediator; }
protected:
    IMediator* m_mediator;
};

class IMediator {
public:
    virtual void notify(IMediatorBase* sender, QString event, QVariant value = QVariant()) = 0;
};

#endif // IMEDIATOR_H
