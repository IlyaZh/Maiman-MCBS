#ifndef DEVCOMMAND_H
#define DEVCOMMAND_H

#include <QObject>
#include "device.h"

class Device;

class DevCommand : public QObject
{
    Q_OBJECT
public:
    static double convertCelToFar(double value);
    static double convertFarToCel(double value);

    explicit DevCommand(Device* device, quint16 code, QString unit = "", double divider = 1, quint8 tol = 0, bool isSigned = false, QObject *parent = nullptr);
    virtual void execute(int value);
    virtual void execute(double value);
    quint16 code();
    QString unit();
    double getDivider();
    quint8 getTol();
    virtual bool isSigned();

signals:
    virtual void newValue(double value);
    virtual void newValue(int value);

public slots:
    void setRawValue(quint16 value);


private:
    Device* m_device;
    quint16 m_code;
    quint16 m_rawValue;
    QString m_unit;
    quint8 m_tol;
    double m_divider;
    bool m_isSigned;

    double value();

};

#endif // DEVCOMMAND_H
