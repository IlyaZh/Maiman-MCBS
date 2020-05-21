#ifndef MODBUSOBSERVERINTERFACE_H
#define MODBUSOBSERVERINTERFACE_H

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

class ModbusObserverInterface {
public:
    ModbusObserverInterface();
    virtual ~ModbusObserverInterface();
    virtual void notify(quint8 addr, quint16 reg, quint16 value) = 0;
};

#endif // MODBUSOBSERVERINTERFACE_H
