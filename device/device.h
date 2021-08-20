#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include "devcommand.h"
#include "devicemodel.h"
#include "model/device/devicepollrequest.h"
#include "model/device/devicewidget.h"
#include <QTimer>
#include <QWidget>
#include <QPointer>

struct DeviceDelays;
struct DeviceModel;
class Device;
struct DeviceConfig;
class DevCommand;

class Device : public QObject
{
    Q_OBJECT
public:
//    explicit Device(quint16 id, quint8 addr, const QString& name, const DeviceDelays &delays, const QVector<DevCommandBuilder*> &commandsBld, QObject *parent = nullptr);
    explicit Device(quint8 addr, const DeviceModel& config, QObject *parent = nullptr);
    ~Device();
    void dataIncome(quint16 reg, quint16 value);
    void dataOutcome(quint16 reg, quint16 value);
    void setWidget(QWidget& widget);
    void destroy();
    QString name();
    quint16 id();
    quint8 addr();
    DevicePollRequest* nextPollRequest();
    bool isLink();
    void clearLink();
    void addWidget(DeviceWidget& widget);
    void removeWidget(DeviceWidget& widget);
//    QVector<const DevCommand*>* getCommands();
//    void connectWidget(DeviceWidget* widget, int code);
//    void disconnectWidget(DeviceWidget* widget, int code = -1);

private:
//    QTimer* timeoutTimer;
    bool m_isLink = false;
    quint8 m_addr = 0;
    quint16 m_Id;
    const QString m_Name;
    const DeviceDelays& m_Delays;
    QMap<quint16, DevCommand*> m_Commands;
    QVector<DevicePollRequest*> m_cmdRequests;
    QVector<DevicePollRequest*>::iterator m_cmdReqItt;
    QVector<Device*> m_childDevices;
    QVector<DeviceWidget*> m_deviceWidgets;

    void createCommandsRequests();

private slots:
//    void dataFromWidget(quint16 reg, double value);
//    void timeout();

signals:
    void dataToModel(quint8 addr, quint16 reg, quint16 value);
    void link(bool);
//    void dataToView(quint16 reg, int value);
//    void dataToView(quint16 reg, double value);

};

#endif // DEVICE_H
