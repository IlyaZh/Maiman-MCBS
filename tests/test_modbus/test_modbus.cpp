#include "test_modbus.h"
#include <QtTest/QtTest>
#include "datasource_mock.h"
#include "network/protocols/modbusprotocol.h"

Test_Modbus::Test_Modbus(QObject *parent) : QObject(parent)
{
    timeout = 1000;

    m_queue.append({1, 1, 10});
}

void Test_Modbus::initTestCase() {
    m_dataSource = new DataSource_Mock(20, 300);
    m_protocol = new  ModbusProtocol(m_dataSource, SoftProtocol::TimeoutDefault, 500);
    QVERIFY2(!m_dataSource.isNull(), "Can't allocate DataSource_Mock object");
    QVERIFY2(m_protocol != nullptr, "Can't allocate ModbusProtocol object");

    m_protocol->Attach(this);
    m_protocol->setDevice(*m_dataSource);
}

void Test_Modbus::test_DataFromSource() {
    lastPackage = m_queue.dequeue();
    m_protocol->setDataValue(lastPackage.addr, lastPackage.reg, lastPackage.value);
    waitForRx = true;
    timer.singleShot(timeout, this, [=]() {
        timeoutSlot();
    });
}

void Test_Modbus::test_DataToSource() {}

void Test_Modbus::cleanupTestCase() {
    delete m_protocol;
    m_protocol = nullptr;
    delete m_dataSource;
    m_dataSource = nullptr;
    QVERIFY2(m_protocol == nullptr, "Modbus object wasn't deleted successfull!");
    QVERIFY2(m_dataSource.isNull(), "DataSource object wasn't deleted successfull!");
}

// private methods

void Test_Modbus::update(quint8 addr, quint16 reg, quint16 value) {
    QVERIFY((addr == lastPackage.addr && reg == lastPackage.reg && value == lastPackage.value));
}

void Test_Modbus::iamReady() {

}

void Test_Modbus::errorOccured(const QString &msg) {

}

void Test_Modbus::timeoutSlot() {
    waitForRx = false;
}
