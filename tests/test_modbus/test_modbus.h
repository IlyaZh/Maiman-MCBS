#ifndef TEST_MODBUS_H
#define TEST_MODBUS_H

#include <QObject>
#include <QPointer>
#include <QTimer>
#include <QQueue>
#include "network/SoftProtocol.h"

struct Package {
    quint8 addr;
    quint16 reg;
    quint16 value;
};

class Test_Modbus : public QObject, public ISoftProtocolObserver
{
    Q_OBJECT
public:
    explicit Test_Modbus(QObject *parent = nullptr);
private slots: // тесты
    void initTestCase(); // типа конструтора, запускается перед тестом, тоже показывается в тесте, т.е. можно проверять, инициализированны ли объекты?
    void test_DataFromSource();
    void test_DataToSource();
    void cleanupTestCase(); // типа деструктора, запускается после, тут чистим всё лишнее

private:
    void update(quint8 addr, quint16 reg, quint16 value) override;
    void iamReady() override;
    void errorOccured(const QString& msg) override;
    void timeoutSlot();

    SoftProtocol* m_protocol = nullptr;
    QPointer<IDataSource> m_dataSource;
    QTimer timer;
    int timeout;
    bool waitForRx = false;
    QQueue<Package> m_queue;
    Package lastPackage;

};

#endif // TEST_MODBUS_H
