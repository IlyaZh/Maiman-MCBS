#pragma once

#include <QObject>
#include <QPointer>
#include <QTimer>
#include <QQueue>
//#include "network/protocols/modbusprotocol.h"
#include "network/SoftProtocol.h"

struct Package {
    quint8 addr;
    quint16 reg;
    quint16 value;
};

class Test_Modbus : public QObject
{
    Q_OBJECT
public:
    explicit Test_Modbus(QObject *parent = nullptr);
private slots: // тесты
    void initTestCase(); // типа конструтора, запускается перед тестом, тоже показывается в тесте, т.е. можно проверять, инициализированны ли объекты?
    void test_GetData();
    void test_SetData();
    void test_SetZeroData();
    void test_Execute();
    void cleanupTestCase();

private:
    QScopedPointer<SoftProtocol> m_protocol;
    bool waitForRx = false;
    QVector<Package> m_queue;
    QVector<QByteArray> m_result;
    Package lastPackage;

};
