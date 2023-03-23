#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include "commandsettings.h"
#include "devicemodel.h"
#include "model/device/devicepollrequest.h"
#include "model/device/devicewidget.h"
#include "device/connectionpolling.h"
#include <QtWidgets>

struct DeviceDelays;
struct DeviceModel;
class Device;
struct CommandSettings;
class DevCommand;

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(quint8 addr, const DeviceModel& config, QObject *parent = nullptr);
    ~Device();
    void dataIncome(quint16 reg, quint16 value);
    QString name();
    quint16 id();
    quint8 addr();
    std::optional<DevicePollRequest> nextPollRequest(bool unconditional = false);
    bool isLink();
    const QMap<quint16, QSharedPointer<DevCommand>>& commands();
    void changeTemperatureUnit(Const::TemperatureUnitId id);
    void unlink();
    void resetConnectionPolling(){m_connectionPolling.reset();};
private:
    bool m_isLink {true};
    int m_addr = 0;
    int m_Id;
    const QString m_Name;
    const DeviceDelays& m_Delays;
    QMap<quint16, QSharedPointer<DevCommand>> m_Commands;
    QVector<DevicePollRequest> m_cmdRequests;
    int m_cmdReqIt = 0;
    QVector<Device*> m_childDevices;
    ConnectionPolling m_connectionPolling;
    void createCommandsRequests();

private slots:
    void dataFromCommand(quint16 reg, quint16 value);

signals:
    void dataToModel(quint8 addr, quint16 reg, quint16 value);
    void linkChanged(bool);

};

#endif // DEVICE_H
