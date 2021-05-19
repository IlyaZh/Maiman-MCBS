#include "devcommand.h"
#include <QtMath>

// static methods

double DevCommand::convertCelToFar(double value) {
    return value * 9.0 / 5.0 + 32.0;
}

double DevCommand::convertFarToCel(double value) {
    return (value - 32.0) * 5.0 / 9.0;
}

// public methods
DevCommand::DevCommand(Device* device, quint16 code, QString unit, double divider, quint8 tol, bool isSigned, QObject *parent) :
    QObject(parent),
    m_device(device),
    m_code(code),
    m_unit(unit),
    m_tol(tol),
    m_divider(divider),
    m_isSigned(isSigned)
{
    m_rawValue = 0;
}

void DevCommand::execute(int value) {
    quint16 inValue = 0;
    if(isSigned()) {
        inValue = (quint16) value;
    } else {
        inValue = value;
    }

    m_device->dataOutcome(m_code, inValue);
}

void DevCommand::execute(double value) {
    quint16 inValue = qRound(value*m_divider);
    if(isSigned()) {
        inValue = static_cast<quint16>((qint16) qRound(value*m_divider));
    } else {
        inValue = static_cast<quint16>(qRound(value*m_divider));
    }

    m_device->dataOutcome(m_code, inValue);
}

quint16 DevCommand::code() {
    return m_code;
}

QString DevCommand::unit() {
    return m_unit;
}

double DevCommand::getDivider() {
    return m_divider;
}

quint8 DevCommand::getTol() {
    return m_tol;
}

bool DevCommand::isSigned() {
    return m_isSigned;
}


// public slots
void DevCommand::setRawValue(quint16 value) {
    m_rawValue = value;
    double dValue = value;

    if(isSigned()) {
        emit newValue((int16_t) value);
        emit newValue((int16_t) value);
    } else {
        emit newValue(value);
        emit newValue(dValue);
    }
}

// private methods
double DevCommand::value() {

    if(isSigned()) {
        double d = static_cast<double>((int16_t) m_rawValue);
        return qRound(d/m_divider*qPow(10,m_tol)-0.5)/qPow(10,m_tol);
    } else {
        double d = static_cast<double>(m_rawValue);
        return qRound(d/m_divider*qPow(10,m_tol)-0.5)/qPow(10,m_tol);
    }
}
