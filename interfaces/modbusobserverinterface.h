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
    explicit ModbusObserverInterface() {}
    virtual ~ModbusObserverInterface() {};
    // Вызывается, когда пришли данные из модбаса
    virtual void modbusNotify(quint8 addr, quint16 reg, quint16 value) = 0;
    // Вызывается модбасом, когда он все передал и у него нет работы
    virtual void modbusReady() = 0;
};

#endif // MODBUSOBSERVERINTERFACE_H
