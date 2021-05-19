#ifndef DEVICEFACTORY_H
#define DEVICEFACTORY_H

#include "parser.h"

class DeviceFactory
{
public:
    explicit DeviceFactory(Parser *model);
    ~DeviceFactory();

    QMap<QString, Command> getCommands(QString id);
    QMap<QString, QString> getIDDevices();
    QStringList getBaudrate();


private:
    /*
    void* m_parser;
    QThread* m_thread;
    QVector<DeviceModel*> m_DeviceModels;

    DeviceModel* findModel(quint16 id);
    */
    Parser *m_parser;
    QVector<device_t> m_device;
    QStringList baudrate;
    QMap<QString, QString> IDDevice;
    void start();
    void CommandReader(int row);
    QModelIndex indexStepBack(int row,int column);
    QModelIndex SForward(int row,int column,QModelIndex parent);
    QString findDeviceID(QString id,int* row);
    QModelIndex findTag(QString tag, QModelIndex parent);
    QVariant AttributesValue(QString attribName, QModelIndex index);
public slots:
    void parsingFinished();

};

#endif // DEVICEFACTORY_H
