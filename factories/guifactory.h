#ifndef GUIFACTORY_H
#define GUIFACTORY_H

#include "parser.h"

class GUIfactory : public QObject
{
public:
    GUIfactory(Parser *model);
    ~GUIfactory();


    device_t getDevice(QString id);
    //Device* createDevice(quint16 id);



public slots:

    void parsingFinished();
private:
    /*
    void* m_parser;
    QThread* m_thread;
    QVector<DeviceModel*> m_DeviceModels;

    DeviceModel* findModel(quint16 id);
    */
    Parser *m_parser;
    QVector<device_t> m_device;
    void start();
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
