#ifndef PROTOCOLOBSERVERINTERFACE_H
#define PROTOCOLOBSERVERINTERFACE_H

#include <QObject>

class ProtocolObserverInterface {
public:
    explicit ProtocolObserverInterface() {}
    virtual ~ProtocolObserverInterface() {};
    // Вызывается, когда пришли данные из модбаса
    virtual void dataIncome(quint8 addr, quint16 reg, quint16 value) = 0;
    // Вызывается модбасом, когда он все передал и у него нет работы
    virtual void dataReady() = 0;
    virtual void timeout(quint8 code) = 0;

    virtual void errorOccured(QString msg) = 0;
    virtual void deviceOpen(bool state) = 0;
};

#endif // PROTOCOLOBSERVERINTERFACE_H
