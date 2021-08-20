#ifndef DEVCOMMAND_H
#define DEVCOMMAND_H

#include <QObject>
#include "device.h"

class Device;
class DevCommand;

struct DeviceConfig {
    explicit DeviceConfig(quint16 code, QString unit, double divider, quint8 tolerance, uint interval, bool isSigned, bool isTemperature);
    DevCommand* makeCommand(Device* pDevice);
    quint16 code;
    QString unit;
    double divider = 1;
    quint8 tolerance = 0;
    bool isSigned = false;
    bool isTemperature = false;
    uint interval;
};

class DevCommand : public QObject
{
    Q_OBJECT
public:
    static double convertCelToFar(double value);
    static double convertFarToCel(double value);

    explicit DevCommand(Device* device, const DeviceConfig& conf, QObject *parent = nullptr);
    const DeviceConfig& config;
//    virtual void execute(int value);
//    virtual void execute(double value);
//    quint16 code();
//    QString unit();
//    double divider();
//    quint8 tolerance();
//    uint interval();
//    uint stepInterval();
//    bool nextInterval();
    virtual bool isSigned();
    double value();


signals:
    void newValue(quint16 code, double value);
//    void newValue(quint16 code, int value);

public slots:
    void setRawValue(quint16 value);


private:
    Device* m_device = nullptr;

    quint16 m_rawValue = 0;
    double m_value = 0;
    uint m_stepInterval = 0;



};

#endif // DEVCOMMAND_H
