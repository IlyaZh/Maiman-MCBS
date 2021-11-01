#pragma once

#include <QObject>
#include <QTime>
#include <QQueue>
#include <QPair>

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
    static double convertCelToFar(double value);
    static double convertFarToCel(double value);

    DevCommand(const CommandSettings& conf);
    quint16 code();
    QString unit();
    double divider();
    int tolerance();
    quint16 getRawFromValue(double value);
    bool isSigned();
    double valueDouble();
    uint valueInt();
    QString valueStr();
    uint interval();


signals:
    void updatedValue();
    void sendValueSignal(quint16 code, quint16 value);
public slots:
    void setFromDevice(quint16 value);
    void setFromWidget(int value);
    void setFromWidget(double value);


private:
    const CommandSettings config;

    quint16 m_rawValue = 0;
    double m_value = 0;
    int m_iValue = 0;
    uint m_stepInterval = 0;
    QString m_strValue;
    QVector<QPair<QTime, double>> m_logValues;
    int m_cmdIt = 0;


};
