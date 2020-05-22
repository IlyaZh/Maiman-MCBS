#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <QObject>
#include <QPointer>
#include <QIODevice>
#include <QVector>
#include "modbus/modbus.h"
#include "interfaces/modbusobserverinterface.h"

class NetworkController2ModelInterface {
public:
    NetworkController2ModelInterface();
    virtual ~NetworkController2ModelInterface();
    virtual void start(QIODevice* networkDevice, QString deviceName) = 0;
    virtual void stop() = 0;
    virtual void setDeviceCommand(quint8 addr, quint16 command, quint16 value);
    virtual void rescanNetwork();
};

class NetworkModel : public QObject, ModbusObserverInterface, NetworkController2ModelInterface
{
    Q_OBJECT
public:
    static const quint16 IDENTIFY_REG_ID_DEFAULT;
    static const quint16 TIMEOUT_MS;
    explicit NetworkModel(QObject *parent = nullptr);
    ~NetworkModel();
    void start(QIODevice* networkDevice, QString deviceName) override;
    void stop() override;
    void setDeviceCommand(quint8 addr, quint16 command, quint16 value) override;
    void rescanNetwork() override;

    void modbusNotify(quint8 addr, quint16 reg, quint16 value) override;
    void modbusReady() override;
signals:

public slots:
private slots:


private:
    QPointer<Modbus> m_netDevice;
    QVector<void*> m_devices; // device models. Change the type of

    void clear();
    void initDevice(quint8 addr, quint16 id);
};

#endif // NETWORKMODEL_H
