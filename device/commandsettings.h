#pragma once

#include <QObject>

class DevCommand;

struct CommandSettings {
    explicit CommandSettings(quint16 code, QString unit, double divider, quint8 tolerance, uint interval, bool isSigned, bool isTemperature);
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

    DevCommand(const CommandSettings& conf);
    quint16 code();
    QString unit();
    double divider();
    int tolerance();
    quint16 getRawFromValue(double value);
    bool isSigned();
    double valueDouble();
    uint valueUInt();
    uint interval();


signals:
    void updatedValue();
    void sendValueSignal(quint16 code, quint16 value);
public slots:
    void setRawValue(quint16 value);
    void sendValue(int value);
    void sendValue(double value);


private:
    const CommandSettings config;

    quint16 m_rawValue = 0;
    double m_value = 0;
    uint m_stepInterval = 0;



};
