#ifndef DEVICE_H
#define DEVICE_H

#include "globals.h"
#include <QObject>
#include <QString>
#include <QXmlStreamReader>
#include <QFile>
#include <QMap>
#include <QCheckBox>
#include <QPushButton>

class xmlReader : public QObject
{
    Q_OBJECT
public:
    explicit xmlReader(QObject *parent = nullptr);

private:
    void setConfigFile(QString file);
    void setDeviceOptions(device_t &device, quint16 dev_id);
    void setDeviceList(QList<availableDev_t>& listPtr);
    void setBaudsList(QList<uint> &baudList);
    QFile *file;
    QString filePath;
    bool readFile();
    bool parseDevice();
    void parseContent();
    void parseCommands();
    void parseControls();
    void parseLeds();
    void parseLedMask(leds_t*);
    void parseParam();
    void parseLimits();
    void parseCalibration();
    void parseButtons();
    void parseBinaryOptions();
    QXmlStreamReader xml;
    quint16 deviceId;
    device_t *device;
    QList<availableDev_t>* availableListPtr;
    QList<uint>* bauds;
    void writeToLog(QString);

signals:
    void errorHandler(QString);
    void endOfLoadingConfig(bool);
    void endOfLoadingProgramConfig(bool);
    void logger(QString);


public slots:
    void parseCommonConfig(QString fileName, QList<availableDev_t>& deviceList, QList<uint> &baudList);
    void parseDeviceConfig(QString fileName, device_t &devicePtr, quint16 dev_id);


};

#endif // DEVICE_H
