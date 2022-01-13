#ifndef SIGNEDCOMMAND_H
#define SIGNEDCOMMAND_H

#include <QObject>
#include "devicecommand.h"
#include <QVariant>

class DeviceSignedCommand : public DeviceCommand
{
public:
    explicit DeviceSignedCommand(const QString& code, const QString& unit = "", double divider = 1, quint8 interval = 1, quint8 tol = 0, bool isTemperature = false, quint16 maxInterval = MAX_COM_INTERVAL_COUNTER, QObject* parent = nullptr);
    virtual DeviceSignedCommand* copy() override;
    bool isSignedValue() const override;
    quint16 getRawValue() const override;
    quint16 getRawFromValue(double value) override;
//    double getValue() override;

public slots:
    void setRawValue(quint16 rawValue) override;
};

#endif // SIGNEDCOMMAND_H
