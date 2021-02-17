#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <QObject>
#include <QPointer>
#include <QIODevice>
#include <QVector>
#include "modbus/modbus.h"
#include "interfaces/modbusobserverinterface.h"
#include "model/ModelInterface.h"

class NetworkModel : public QObject, ModbusObserverInterface, public ModelInterface
{
    Q_OBJECT
public:
    static const quint16 IDENTIFY_REG_ID_DEFAULT;
    static const quint16 TIMEOUT_MS;
    explicit NetworkModel(QObject *parent = nullptr);
    ~NetworkModel() override;
    void start(int protocol, QString host, int port) override;
    bool isStart() override;
    void stop() override;
    void setDeviceCommand(quint8 addr, quint16 command, quint16 value) override;
    void rescanNetwork() override;

    void modbusNotify(quint8 addr, quint16 reg, quint16 value) override;
    void modbusReady() override;
signals:
    void modelConnected(bool);

public slots:
private slots:


private:
    QPointer<Modbus> m_netDevice;
    QVector<void*> m_devices; // device models. Change the type of
    bool m_bIsStart;


    void clear();
    void initDevice(quint8 addr, quint16 id);
};


#endif // NETWORKMODEL_H
