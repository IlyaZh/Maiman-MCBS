#ifndef DEVCOMMAND_H
#define DEVCOMMAND_H

#include <QObject>
#include <QVariant>

class DevCommand {
public:
    DevCommand(quint16 reg, int divider);
    virtual ~DevCommand();

    quint16 reg() { return m_reg; }
    virtual int toInt() = 0;
    quint16 toUInt() { return static_cast<quint16>(m_value.toUInt()); }
    QString name() { return m_name; }
    QString unit() { return m_unit; }
    double toDouble();


protected:
    QString m_name;
    QString m_unit;
    quint16 m_reg;
    int m_divider;
    QVariant m_value;
    double m_dValue;
};



#endif // DEVCOMMAND_H
