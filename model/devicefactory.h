#ifndef DEVICEMODELFACTORY_H
#define DEVICEMODELFACTORY_H

#include "device/devicemodel.h"
#include "device/device.h"
#include <QThread>
#include <QObject>
#include "factories/parserworker.h"
#include "appsettings.h"


class DeviceFactory : public QObject
{
    Q_OBJECT
public:
    DeviceFactory(QString fileName, AppSettings& settings, QObject *parent = nullptr);
    ~DeviceFactory();
    void start();
    Device* createDevice(quint8 addr, quint16 id);
    QStringList getBaudrate();
    QVector<QPair<uint, QString>> getCommonDeviceIDs();

private slots:
    void parsingFinished();
    void threadError(const QString&);
private:
    QThread* m_thread;
    AppSettings& m_settings;
    QString m_fileName;

    DeviceModel* findModel(quint16 id);
    bool parseTree(const TreeItem& tree);
    QString parseBaudRate(const TreeItem& item);
    QVector<QPair<uint, QString>> parseCommonDevId(const TreeItem& item);
    DeviceModel* parseDevice(const TreeItem& item);
    QVector<DevCommandBuilder*>* parseCommands(const TreeItem& item);


    QPointer<ParserWorker> m_parseWorker;
    QVector<DeviceModel*> m_DeviceModels;
//    QVector<device_t> m_device;
    QStringList m_baudrates;
    QVector<QPair<uint, QString>> m_commondDevicesId;
//    QMap<QString, QString> IDDevice;


};

#endif // DEVICEMODELFACTORY_H
