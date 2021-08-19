#include "devcommand.h"
#include <QtMath>

// DevCommandBuilder class
DevCommandBuilder::DevCommandBuilder(quint16 code, QString unit, double divider, quint8 tol, uint interval, bool isSigned, bool isTemperature)
{
    m_code = code;
    m_unit = unit;
    m_divider = divider;
    m_tol = tol;
    m_isSigned = isSigned;
    m_isTemperature = isTemperature;
    m_interval = interval;
}

DevCommand* DevCommandBuilder::makeCommand(Device* pDevice) {
    return new DevCommand(pDevice, m_code, m_unit, m_divider, m_tol, m_interval, m_isSigned, m_isTemperature);
}

quint16 DevCommandBuilder::code() const {
    return m_code;
}

uint DevCommandBuilder::interval() const {
    return m_interval;
}

// DevCommand class
// static methods

double DevCommand::convertCelToFar(double value) {
    return value * 9.0 / 5.0 + 32.0;
}

double DevCommand::convertFarToCel(double value) {
    return (value - 32.0) * 5.0 / 9.0;
}

// public methods
DevCommand::DevCommand(Device* device, quint16 code, QString unit, double divider, quint8 tol, uint interval, bool isSigned, bool isTemperature, QObject *parent) :
    QObject(parent),
    m_device(device),
    m_code(code),
    m_unit(unit),
    m_tol(tol),
    m_divider(divider),
    m_isSigned(isSigned),
    m_isTemperature(isTemperature)
{
    m_rawValue = 0;

    m_unit.replace("(deg)", QString('\370'), Qt::CaseInsensitive);
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

double DevCommand::divider() {
    return m_divider;
}

quint8 DevCommand::tolerance() {
    return m_tol;
}

uint DevCommand::interval() {
    return m_interval;
}

//uint DevCommand::stepInterval() {
//    return m_stepInterval;
//}

//bool DevCommand::nextInterval() {
//    m_stepInterval++;
//    bool state = (m_stepInterval == m_interval);

//    if(state) {
//        m_stepInterval = 0;
//    }
//    return state;
//}

bool DevCommand::isSigned() {
    return m_isSigned;
}


// public slots
void DevCommand::setRawValue(quint16 value) {
    QVariant variant(value);
    m_rawValue = value;

    int iValue = (isSigned()) ? static_cast<qint16>(variant.toInt()) : static_cast<quint16>(variant.toUInt());
    double dValue = (static_cast<double>(iValue) / divider());
    QString tempStr;
    tempStr.number(dValue, 'g', tolerance());

//    emit newValue(m_code, tempStr.toDouble());
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
