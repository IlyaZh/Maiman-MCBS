#pragma once

#include <QObject>
#include "device.h"

class Device;
struct DevCommand;

struct CommandSettings {
    explicit CommandSettings(quint16 code, QString unit, double divider, quint8 tolerance, uint interval, bool isSigned, bool isTemperature);
//    DevCommand* makeCommand(Device* pDevice);
    quint16 code;
    QString unit;
    double divider = 1;
    quint8 tolerance = 0;
    bool isSigned = false;
    bool isTemperature = false;
    uint interval;
};

struct DevCommand
{
public:
    static double convertCelToFar(double value);
    static double convertFarToCel(double value);

    DevCommand(/*Device* device, */const CommandSettings& conf);
    const CommandSettings config;
//    virtual void execute(int value);
//    virtual void execute(double value);
//    quint16 code();
//    QString unit();
//    double divider();
//    quint8 tolerance();
//    uint interval();
//    uint stepInterval();
//    bool nextInterval();
    quint16 getRawFromValue(double value);
    bool isSigned();
    double value();


//signals:
//    void newValue(quint16 code, double value);
//    void newValue(quint16 code, int value);

//public slots:
    void setRawValue(quint16 value);


private:
//    Device* m_device = nullptr;

    quint16 m_rawValue = 0;
    double m_value = 0;
    uint m_stepInterval = 0;



};
