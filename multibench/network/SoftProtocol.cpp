#include "SoftProtocol.h"
#include <QDebug>
#include <QDateTime>

const quint8 SoftProtocol::MaxAddress = 32;

quint8 SoftProtocol::hiBYTE(quint16 value) {
    return ((value >> 8) & 0xff);
}

quint8 SoftProtocol::loBYTE(quint16 value) {
    return (value & 0xff);
}

bool SoftProtocol::isError() { return m_error; }

QString SoftProtocol::errorString() { return m_errorString; }
