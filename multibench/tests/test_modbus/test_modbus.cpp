#include "test_modbus.h"
#include <QtTest/QtTest>
#include "network/protocols/modbusprotocol.h"

Test_Modbus::Test_Modbus(QObject *parent) :
    QObject(parent),
    lastPackage({0,0,0})
{
    m_queue.append({1, 10, 2});
    m_queue.append({2, 3, 80});
    m_queue.append({0, 1, 255});


    QByteArray res;
    res.append(static_cast<char>(0x01));
    res.append(static_cast<char>(0x03));
    res.append(static_cast<char>(0x00));
    res.append(static_cast<char>(0x0A));
    res.append(static_cast<char>(0x00));
    res.append(static_cast<char>(0x02));
    res.append(static_cast<char>(0xe4));
    res.append(static_cast<char>(0x09));
    m_result.append(res);

    res.clear();
    res.append(static_cast<char>(0x02));
    res.append(static_cast<char>(0x06));
    res.append(static_cast<char>(0x00));
    res.append(static_cast<char>(0x03));
    res.append(static_cast<char>(0x00));
    res.append(static_cast<char>(0x50));
    res.append(static_cast<char>(0x79));
    res.append(static_cast<char>(0xc5));
    m_result.append(res);

    res.clear();
    res.append(static_cast<char>(0x00));
    res.append(static_cast<char>(0x06));
    res.append(static_cast<char>(0x00));
    res.append(static_cast<char>(0x01));
    res.append(static_cast<char>(0x00));
    res.append(static_cast<char>(0xff));
    res.append(static_cast<char>(0x99));
    res.append(static_cast<char>(0x9b));
    m_result.append(res);
}

void Test_Modbus::initTestCase() {
    m_protocol.reset(new ModbusProtocol(this));
    QVERIFY2(m_protocol != nullptr, "Can't allocate ModbusProtocol object");
}

void Test_Modbus::test_GetData() {
    int key = 0;
    Package pack = m_queue.at(key);
    QByteArray res = m_protocol->getDataValue(pack.addr, pack.reg, pack.value);
    QVERIFY(res == m_result.at(key));
}

void Test_Modbus::test_SetData() {
    int key = 1;
    Package pack = m_queue.at(key);
    QByteArray res = m_protocol->setDataValue(pack.addr, pack.reg, pack.value);
    QVERIFY(res == m_result.at(key));
}

void Test_Modbus::test_SetZeroData() {
    int key = 2;
    Package pack = m_queue.at(key);
    QByteArray res = m_protocol->setDataValue(pack.addr, pack.reg, pack.value);
    QVERIFY(res == m_result.at(key));
}

void Test_Modbus::test_Execute() {
    QByteArray toDevice;
    toDevice.append(static_cast<char>(0x03));
    toDevice.append(static_cast<char>(0x03));
    toDevice.append(static_cast<char>(0x00));
    toDevice.append(static_cast<char>(0x40));
    toDevice.append(static_cast<char>(0x00));
    toDevice.append(static_cast<char>(0x02));
    toDevice.append(static_cast<char>(0xc4));
    toDevice.append(static_cast<char>(0x3d));

    QByteArray fromDevice;
    fromDevice.append(static_cast<char>(0x03));
    fromDevice.append(static_cast<char>(0x03));
    fromDevice.append(static_cast<char>(0x04));
    fromDevice.append(static_cast<char>(0x00));
    fromDevice.append(static_cast<char>(0x00));
    fromDevice.append(static_cast<char>(0x00));
    fromDevice.append(static_cast<char>(0x00));
    fromDevice.append(static_cast<char>(0xD9));
    fromDevice.append(static_cast<char>(0xF3));

    ModbusProtocol::DataVector result = m_protocol->execute(fromDevice, toDevice);
    ModbusProtocol::DataVector temp;
    temp.push_back({3, 64, 0});
    temp.push_back({3, 65, 0});
    QVERIFY2(temp==result, "Data isn't equals!");
}

void Test_Modbus::cleanupTestCase() {
    m_protocol.reset();
    QVERIFY2(m_protocol.isNull(), "Can't delete ModbusProtocol object");
}
