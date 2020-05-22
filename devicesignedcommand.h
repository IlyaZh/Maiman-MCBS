#ifndef SIGNEDCOMMAND_H
#define SIGNEDCOMMAND_H

#include <QObject>
#include "devicecommand.h"
#include <QVariant>

class DeviceSignedCommand : public DeviceCommand
{
public:
    explicit DeviceSignedCommand(QString code, QString unit = "", double divider = 1, quint8 interval = 1, bool isTemperature = false, QObject* parent = nullptr);
    bool isSignedValue() override;
    quint16 getRawValue() override;
    int getRawFromValue(double value) override;
//    double getValue() override;

public slots:
    void setRawValue(quint16 rawValue) override;
};

#endif // SIGNEDCOMMAND_H
