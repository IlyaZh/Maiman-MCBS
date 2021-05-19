#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include "devcommand.h"
#include "devicemodel.h"
#include "model/device/parameterwidget.h"
#include "model/device/devicepollrequest.h"
#include "model/device/devicewidget.h"
#include <QTimer>

class DeviceDelays;
class DeviceModel;
class Device;
class DevCommand;

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(quint16 id, quint8 addr, QString name, DeviceDelays *delays, QVector<DevCommand*> *commands, QObject *parent = nullptr);
    void dataIncome(quint16 reg, quint16 value);
    void dataOutcome(quint16 reg, quint16 value);
    QString name();
    quint16 id();
    quint8 addr();
    DevicePollRequest* nextPollRequest();
    bool isLink();
//    QVector<const DeviceCommand*>* getCommands();
//    void connectWidget(DeviceWidget* widget, int code);
//    void disconnectWidget(DeviceWidget* widget, int code = -1);

private:
    QTimer* timeoutTimer;
    bool m_isLink;
    quint8 m_addr;
    quint16 m_Id;
    QString m_Name;
    DeviceDelays* m_Delays;
    QMap<quint16, DevCommand*> *m_Commands;
    QVector<DevicePollRequest*> m_cmdRequests;
    QVector<DevicePollRequest*>::iterator m_cmdReqItt;
    QVector<Device*> m_childDevices;
//    QVector<DeviceWidget*> m_deviceWidgets;

    void createCommandsRequests();

private slots:
    void timeout();

signals:
    void dataToModel(quint8 addr, quint16 reg, quint16 value);
//    void dataToView(quint16 reg, quint16 value);
    void dataToView(quint16 reg, double value);
    void setLink(bool);

};

#endif // DEVICE_H
