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

#include "devicemodel.h"

//class AvailableDevice_t {
//public:
//    AvailableDevice_t(QString name, quint16 id) {
//        m_name = name;
//        m_id = id;
//    }
//    QString name() { return m_name; }
//    quint16 id() { return m_id; }
//private:
//    QString m_name;
//    quint16 m_id;
//};

class xmlReader : public QObject
{
    Q_OBJECT
public:
    explicit xmlReader(QObject *parent = nullptr);
    ~xmlReader();
    void addConfFileName(QString fileName) { m_deviceConfigsFileList.append(fileName); }
    bool parseProgramConfig(QString filename);
    QList<uint> baudList() { return m_baudList; }
    DeviceModel* createDevice(quint16 id);
private:
    QStringList m_deviceConfigsFileList;
    QString m_programConfigFile;
    QList<uint> m_baudList;
//    QList<const AvailableDevice_t*> m_AvailableDevices;


//    QFile *file;
//    QString filePath;
//    bool readFile();
//    bool parseDevice();
//    void parseContent();
//    void parseCommands();
//    void parseControls();
//    void parseLeds();
//    void parseLedMask(leds_t*);
//    void parseParam();
//    void parseLimits();
//    void parseCalibration();
//    void parseButtons();
//    void parseBinaryOptions();
//    QXmlStreamReader xml;
//    quint16 deviceId;
//    device_t *device;
//    QList<availableDev_t>* availableListPtr;
//    QList<uint>* bauds;
    void writeToLog(QString);

public slots:
    void process(); 	/*  создает и запускает построитель отчетов */
    void stop();    	/*  останавливает построитель отчетов */

signals:
    void finished(); 	/* сигнал о завершении  работы построителя отчетов */
    void errorHandler(QString);
    void endOfLoadingConfig(bool);
    void endOfLoadingProgramConfig(bool);
    void logger(QString);


public slots:



};

#endif // DEVICE_H
