#ifndef DEVICEMODELFACTORY_H
#define DEVICEMODELFACTORY_H

#include "device/devicemodel.h"
#include "device/device.h"
#include <QThread>
#include <QObject>
#include "factories/parser.h"
#include "appsettings.h"


class DeviceFactory : public QObject
{
    Q_OBJECT
public:
    DeviceFactory(Parser* parser, AppSettings* settings, QObject *parent = nullptr);
    ~DeviceFactory();
    void start();
    Device* createDevice(quint8 addr, quint16 id);
    QStringList getBaudrate();

private slots:
    void parsingFinished();
    void threadError(QString);
private:
    QThread* m_thread;
    AppSettings* m_settings;

    DeviceModel* findModel(quint16 id);

    Parser *m_parser;
    QVector<DeviceModel*> m_DeviceModels;
//    QVector<device_t> m_device;
    QStringList baudrate;
//    QMap<QString, QString> IDDevice;

    void CommandReader(int row);
    QModelIndex indexStepBack(int row,int column);
    QModelIndex SForward(int row,int column,QModelIndex parent);
    QString findDeviceID(QString id,int* row);
    QModelIndex findTag(QString tag, QModelIndex parent);
    QVariant AttributesValue(QString attribName, QModelIndex index);


};

#endif // DEVICEMODELFACTORY_H
