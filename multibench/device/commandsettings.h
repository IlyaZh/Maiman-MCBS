#pragma once

#include <QObject>
#include <QTime>
#include <QQueue>
#include <QPair>
#include "constants.h"
#include "appsettings.h"

class DevCommand;

struct CommandSettings {
    explicit CommandSettings(quint16 code, const QString& unit, double divider, quint8 tolerance, uint interval, bool isSigned, bool isTemperature);
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
    static const int maxLogValues;
    static const QSet<quint16> logCommandsSet;
    static double convertCelToFar(double value);
    static double convertFarToCel(double value);

    explicit DevCommand(const CommandSettings& conf);
    void changeTemperatureUnit(Const::TemperatureUnitId id);
    quint16 code() const;
    QString unit() const ;
    double divider() const;
    int tolerance() const;
    bool isSigned() const;
    bool isTemperature() const;
    double valueDouble() const ;
    uint valueInt() const ;
    QString valueStr() const;
    uint interval() const;

//    const QVector<double>& historyValues() const;
    double avgValue() const;
    double maxValue() const;
    double minValue() const;


signals:
    void updatedValue();
    void updatedUnit(QStringView unit);
    void sendValueSignal(quint16 code, quint16 value);
public slots:
    void setFromDevice(quint16 value);
    void setFromWidget(int value);
    void setFromWidget(double value);


private:
    const CommandSettings m_config;

    quint16 m_rawValue = 0;
    double m_value = 0;
    int m_iValue = 0;
    uint m_stepInterval = 0;
    QString m_strValue;
//    QVector<QPair<QTime, double>> m_logValues;
    QScopedPointer<QVector<double>> m_logValues;
    double m_cmdSum = 0;
    int m_cmdIt = 0;
    Const::TemperatureUnitId m_tempId{Const::TemperatureUnitId::Celsius};
    bool firstRun{true};

    quint16 getRawFromValue(double value) const;
};
