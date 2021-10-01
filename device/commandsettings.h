#pragma once

#include <QObject>
#include "device.h"

class Device;
class DevCommand;

struct CommandSettings {
    explicit CommandSettings(quint16 code, QString unit, double divider, quint8 tolerance, uint interval, bool isSigned, bool isTemperature);
//    DevCommand* makeCommand(Device* pDevice);
    quint16 m_code;
    QString m_unit;
    double m_divider = 1;
    quint8 m_tolerance = 0;
    bool m_isSigned = false;
    bool m_isTemperature = false;
    uint m_interval;
};

class DevCommand : public QObject
{
    Q_OBJECT
public:
    static double convertCelToFar(double value);
    static double convertFarToCel(double value);

    DevCommand(Device* device, const CommandSettings& conf);
    const CommandSettings config;
//    virtual void execute(int value);
//    virtual void execute(double value);
    quint16 code();
    QString unit();
    double divider();
    int tolerance();
//    uint interval();
//    uint stepInterval();
//    bool nextInterval();
    quint16 getRawFromValue(double value);
    bool isSigned();
    double valueDouble();
    uint valueUInt();


signals:
    void updatedValue();
    void sendValueSignal(quint16 code, quint16 value);
//    void newValue(quint16 code, double value);
//    void newValue(quint16 code, int value);

public slots:
    void setRawValue(quint16 value);
    void sendValue(int value);
    void sendValue(double value);


private:
    Device* m_device;

    quint16 m_rawValue = 0;
    double m_value = 0;
    uint m_stepInterval = 0;



};
