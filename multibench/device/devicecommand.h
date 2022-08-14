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
    explicit DeviceCommand(QString code, const QString& unit = "", double divider = 1, quint8 interval = 1, quint8 tol = 0, bool isTemperature = false, quint16 maxInterval = MAX_COM_INTERVAL_COUNTER, QObject *parent = nullptr);
    virtual DeviceCommand* copy();
    void setTemperatureUnit(const QString& unit);
    QString getTemperatureUnit() const;
    QString getCodeStr() const;
    quint16 getCode() const;
    virtual double getValue() const;
    virtual qint16 getIValue() const;
    virtual quint16 getRawValue() const;
    bool needToRequest();
    quint8 getInterval() const;
    void resetInterval();
    double getDivider() const;
    quint8 getTol() const;
    virtual bool isSignedValue() const;
    bool isTemperature() const;
    QString getUnit() const;
    virtual quint16 getRawFromValue(double value);

    static double convertCelToFar(double val);
    static double convertFarToCel(double val);

signals:
    void valueChanged(double);

public slots:
    virtual void setRawValue(quint16 value);

protected:
    quint16 maxInterval;
    QString temperatureUnit;
    QString unit;
    QString codeStr;
    quint16 code;
    double Divider;
    bool isTemperatureFlag;
    QVariant rawValue;
    double value;
    int iValue;
    quint8 interval;
    quint8 tick;
    quint8 tol;
};

#endif // PARAMETER_T_H
