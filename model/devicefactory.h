#ifndef DEVICEMODELFACTORY_H
#define DEVICEMODELFACTORY_H

#include "device/devicemodel.h"
#include "device/device.h"
#include <QThread>
#include <QObject>
#include "factories/parserworker.h"
#include "appsettings.h"
#include <QSharedPointer>


class DeviceFactory : public QObject
{
    Q_OBJECT
public:
    DeviceFactory(QString fileName, QSharedPointer<AppSettings> settings, QObject *parent = nullptr);
    ~DeviceFactory();
    void start();
    Device* createDevice(quint8 addr, quint16 id);
    QStringList getBaudrate();

private slots:
    void parsingFinished();
    void threadError(QString);
private:
    QThread* m_thread;
    QSharedPointer<AppSettings> m_settings;
    QString m_fileName;

    DeviceModel* findModel(quint16 id);
    bool parseTree(TreeItem* tree);
    QStringList parseBaudRates(TreeItem* item);
    QVector<QPair<uint, QString>> parseCommonDevId(TreeItem* item);

    QPointer<ParserWorker> m_parseWorker;
    QVector<QSharedPointer<DeviceModel>> m_DeviceModels;
//    QVector<device_t> m_device;
    QStringList m_baudrates;
    QVector<QPair<uint, QString>> m_commondDevicesId;
//    QMap<QString, QString> IDDevice;


};

#endif // DEVICEMODELFACTORY_H
