#ifndef DEVICEMODELFACTORY_H
#define DEVICEMODELFACTORY_H

#include "device/devicemodel.h"
#include "device/device.h"
#include <QThread>
#include <QObject>
#include "factories/Parser.h"
#include "appsettings.h"
#include <QSharedPointer>


class DeviceFactory : public QObject
{
    Q_OBJECT
public:
    DeviceFactory(Parser* parser, QSharedPointer<AppSettings> settings, QObject *parent = nullptr);
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

    DeviceModel* findModel(quint16 id);

    QSharedPointer<Parser> m_parser;
    QVector<QSharedPointer<DeviceModel>> m_DeviceModels;
//    QVector<device_t> m_device;
    QStringList baudrate;
//    QMap<QString, QString> IDDevice;


};

#endif // DEVICEMODELFACTORY_H
