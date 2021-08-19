#ifndef DEVCOMMAND_H
#define DEVCOMMAND_H

#include <QObject>
#include "device.h"

class Device;
class DevCommand;

class DevCommandBuilder {
public:
    DevCommandBuilder(quint16 code, QString unit, double divider, quint8 tol, uint interval, bool isSigned, bool isTemperature);
    DevCommand* makeCommand(Device* pDevice);
    quint16 code() const;
    uint interval() const;
private:
    quint16 m_code;
    QString m_unit;
    double m_divider;
    quint8 m_tol;
    bool m_isSigned;
    bool m_isTemperature;
    uint m_interval;
};

class DevCommand : public QObject
{
    Q_OBJECT
public:
    static double convertCelToFar(double value);
    static double convertFarToCel(double value);

    explicit DevCommand(Device* device, quint16 code, QString unit, double divider, quint8 tol, uint interval, bool isSigned, bool isTemprature, QObject *parent = nullptr);
    virtual void execute(int value);
    virtual void execute(double value);
    quint16 code();
    QString unit();
    double divider();
    quint8 tolerance();
    uint interval();
//    uint stepInterval();
//    bool nextInterval();
    virtual bool isSigned();


signals:
    void newValue(quint16 code, double value);
//    void newValue(quint16 code, int value);

public slots:
    void setRawValue(quint16 value);


private:
    Device* m_device = nullptr;
    quint16 m_code = 0;
    quint16 m_rawValue = 0;
    QString m_unit = "";
    quint8 m_tol = 0;
    double m_divider = 1;
    bool m_isSigned;
    bool m_isTemperature;
    uint m_interval = 1;
    uint m_stepInterval = 0;

    double value();

};

#endif // DEVCOMMAND_H
