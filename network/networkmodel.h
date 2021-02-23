#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <QObject>
#include <QPointer>
#include <QIODevice>
#include <QVector>
#include "protocols/modbus.h"
#include "interfaces/ProtocolObserverInterface.h"
#include "model/ModelInterface.h"
#include "SoftProtocol.h"
//#include "enums.h"

class NetworkModel : public QObject, ProtocolObserverInterface, public ModelInterface
{
    Q_OBJECT
public:
    static const quint16 IDENTIFY_REG_ID_DEFAULT;
    static const quint16 TIMEOUT_MS;
    explicit NetworkModel(SoftProtocol* protocol, QObject *parent = nullptr);
    ~NetworkModel();
    void start(QIODevice* iodevice) override;
    bool isStart() override;
    void stop() override;
    void setDeviceCommand(quint8 addr, quint16 command, quint16 value) override;
    void rescanNetwork() override;

    void dataNotify(quint8 addr, quint16 reg, quint16 value) override;
    void dataReady() override;
signals:
    void modelConnected(bool);

public slots:
private slots:


private:
    QPointer<SoftProtocol> m_netDevice;
    QVector<void*> m_devices; // device models. Change the type of
    bool m_bIsStart;


    void clear();
    void initDevice(quint8 addr, quint16 id);
};


#endif // NETWORKMODEL_H
