#ifndef GUIFACTORY_H
#define GUIFACTORY_H

#include "parser.h"
#include "device/devcommand.h"
#include "appsettings.h"
#include <QSharedPointer>

class GUIfactory : public QObject
{
public:
    GUIfactory(/*Parser* model, */QSharedPointer<AppSettings> settings);
    ~GUIfactory();
    void start();

    device_t getDevice(QString id);
    QWidget* createWidget(quint16 devId, QVector<const DevCommand*>* pCommands);



public slots:

    void parsingFinished();
private:
    /*
    void* m_parser;
    QThread* m_thread;
    QVector<DeviceModel*> m_DeviceModels;

    DeviceModel* findModel(quint16 id);
    */
    QSharedPointer<Parser> m_parser;
    QSharedPointer<AppSettings> m_settings;
    QVector<device_t> m_device;

    QModelIndex indexStepBack(int row,int column);
    QModelIndex SForward(int row,int column,QModelIndex parent);
    QString findDeviceID(QString id,int* row);
    QModelIndex findTag(QString tag, QModelIndex parent);
    QVariant AttributesValue(QString attribName, QModelIndex index);
    void GUIReader(int row);

    void ContentReader(QModelIndex index,device_t device);
    void LimitsReader(QModelIndex index,device_t device);
    void CalibrationKoefReader(QModelIndex index,device_t device);
    void ParamControlReader(QModelIndex index,device_t device);
    void BinaryOptionsReader(QModelIndex index,device_t device);
    void ButtonsReader(QModelIndex index,device_t device);
    void LedsReader(QModelIndex index,device_t device);

};

#endif // GUIFACTORY_H
