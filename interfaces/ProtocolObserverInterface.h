#ifndef PROTOCOLOBSERVERINTERFACE_H
#define PROTOCOLOBSERVERINTERFACE_H

#include <QObject>

//class ModbusObserverInterface : public QObject
//{
//    Q_OBJECT
//public:
//    explicit ModbusObserverInterface(QObject *parent = nullptr) : QObject(parent) {}

//signals:

//public slots:
//    void notify(quint16 reg, quint16 value);
//};

class ProtocolObserverInterface {
public:
    explicit ProtocolObserverInterface() {}
    virtual ~ProtocolObserverInterface() {};
    // Вызывается, когда пришли данные из модбаса
    virtual void dataIncome(quint8 addr, quint16 reg, quint16 value) = 0;
    // Вызывается модбасом, когда он все передал и у него нет работы
    virtual void dataReady() = 0;
};

#endif // PROTOCOLOBSERVERINTERFACE_H
