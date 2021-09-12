#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include "commandsettings.h"
#include "devicemodel.h"
#include "model/device/devicepollrequest.h"
#include "model/device/devicewidget.h"
#include <QTimer>
#include <QWidget>

struct DeviceDelays;
struct DeviceModel;
class Device;
struct CommandSettings;
struct DevCommand;

class Device : public QObject
{
    Q_OBJECT
public:
//    explicit Device(quint16 id, quint8 addr, const QString& name, const DeviceDelays &delays, const QVector<DevCommandBuilder*> &commandsBld, QObject *parent = nullptr);
    explicit Device(quint8 addr, const DeviceModel& config, QObject *parent = nullptr);
    ~Device();
    void dataIncome(quint16 reg, quint16 value);
    void setWidget(QWidget& widget);
    void destroy();
    QString name();
    quint16 id();
    quint8 addr();
    const DevicePollRequest nextPollRequest();
    bool isLink();
    void clearLink();
    void addWidget(DeviceWidget& widget);
    void removeWidget(DeviceWidget& widget);

private:
    bool m_isLink = false;
    int m_addr = 0;
    int m_Id;
    const QString m_Name;
    const DeviceDelays& m_Delays;
    QMap<uint, DevCommand*> m_Commands;
    QVector<DevicePollRequest> m_cmdRequests;
//    QVector<DevicePollRequest>::iterator m_cmdReqItt;
    int m_cmdReqIt = 0;
    QVector<Device*> m_childDevices;
    QVector<DeviceWidget*> m_deviceWidgets;

    void createCommandsRequests();

private slots:
    void dataFromWidget(quint16 reg, double value);

signals:
    void dataToModel(quint8 addr, quint16 reg, quint16 value);
    void link(bool);

};

#endif // DEVICE_H
