#ifndef DEVCOMMAND_H
#define DEVCOMMAND_H

#include <QObject>
#include <QVariant>
#include <qmath.h>

class DevCommand {
public:
    DevCommand(quint16 reg, int divider);
    virtual ~DevCommand();

    quint16 reg() { return m_reg; }
//    virtual int toInt() = 0;
//    virtual qint16 toInt() { return static_cast<qint16>(m_value.toInt()); }
//    virtual quint16 toUInt() { return static_cast<quint16>(m_value.toUInt()); }
    QString name() { return m_name; }
    QString unit() { return m_unit; }
    int tol() { return m_tolerance; }
//    virtual double toDouble() { return static_cast<double>(m_value.toDouble()); }


protected:
    QString m_name;
    QString m_unit;
    quint16 m_reg;
    uint m_divider;
    QVariant m_value;
    double m_dValue;
    uint m_tolerance;
};



#endif // DEVCOMMAND_H
