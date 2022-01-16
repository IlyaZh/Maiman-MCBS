#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <QtWidgets>
#include <QSet>
#include "constants.h"

class MainViewFacade;
class DataSource;
class MainFacade;
class DeviceFactory;
struct DeviceModel;
class SoftProtocol;
class ModbusProtocol;
class Device;
class DevCommand;
class DataThread;
class IDataSource;

class NetworkModel : public QObject
{
    Q_OBJECT
public:
    static const quint16 IDENTIFY_REG_ID_DEFAULT;
    static const quint16 TIMEOUT_MS;
    explicit NetworkModel(DeviceFactory &deviceModelFactory, SoftProtocol& protocol, QObject *parent = nullptr);
    ~NetworkModel();
    void setTimeout(int timeout);
    void start(QScopedPointer<IDataSource>& source);
    bool isStart();
    void stop();
    QMap<quint16, QSharedPointer<DevCommand>> getCommands(quint8 addr);
    void clearNetwork();
public slots:
    void dataOutcome(quint8 addr, quint16 reg, quint16 value);
    void temperatureUnitsChanged(Const::TemperatureUnitId id);
    void rescanNetwork();
    void getBaudrate();
private slots:
    void timeout(const QByteArray& lastPackage);
    void pollRequest();
    void readyRead(const QByteArray& rxPackage, const QByteArray& lastPackage);

signals:
    void signal_createWidgetFor(Device* device);
    void signal_setBaudrateToWindow(QStringList);
    void signal_connected(bool);
    void signal_errorOccured(const QString& error);
    void signal_writeData(const QByteArray& msg, qint64 waitBytes, bool priority);



private:
    DeviceFactory& m_deviceModelFactory;
    SoftProtocol& m_protocol;
    QMap<quint8, QSharedPointer<Device>> m_devices;
    bool m_isStart = false;
    int m_timeoutMs {Const::NetworkTimeoutMSecs::defaultValue};
    QPointer<DataThread> m_worker;
    QQueue<QByteArray> m_queue;
    QQueue<QByteArray> m_priorityQueue;

    void clear();
    void initDevice(quint8 addr, quint16 id);
    void tryToSend();
};


#endif // NETWORKMODEL_H
