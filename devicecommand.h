#ifndef PARAMETER_T_H
#define PARAMETER_T_H

#include <QObject>
#include <QString>
#include <QVariant>

class DeviceCommand : public QObject
{
    Q_OBJECT
public:
    static const quint16 MAX_COM_INTERVAL_COUNTER;
    explicit DeviceCommand(QString code, QString unit = "", double divider = 1, quint8 interval = 1, bool isTemperature = false, QObject *parent = nullptr);
    void setTemperatureUnit(QString unit);
    QString getTemperatureUnit();
    QString getCode();
    virtual double getValue();
    virtual qint16 getIValue();
    virtual quint16 getRawValue();
    bool needToRequest();
    void resetInterval();
    double getDivider();
    virtual bool isSignedValue();
    bool isTemperature();
    QString getUnit();
    virtual quint16 getRawFromValue(double value);

    static double convertCelToFar(double val);
    static double convertFarToCel(double val);

signals:
    void valueChanged();

public slots:
    virtual void setRawValue(quint16 value);

protected:
    QString temperatureUnit;
    QString unit;
    QString Code;
    double Divider;
    bool isTemperatureFlag;
    QVariant rawValue;
    double value;
    int iValue;
    quint8 interval;
    quint8 tick;
};

#endif // PARAMETER_T_H
