#include "globaltest.h"
#include <QTest>
#include "test_modbus/test_modbus.h"

GlobalTest::GlobalTest(int argc, char* argv[], QObject *parent) : QObject(parent)
{
    QTest::qExec(new Test_Modbus(this), argc, argv);

}
