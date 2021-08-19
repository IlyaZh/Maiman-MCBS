#include "SoftProtocol.h"
#include <QDebug>
#include <QDateTime>

// ==============================================================================================
const quint8 SoftProtocol::MaxAddress = 32;

quint8 SoftProtocol::hiBYTE(quint16 value) {
    return ((value >> 8) & 0xff);
}

quint8 SoftProtocol::loBYTE(quint16 value) {
    return (value & 0xff);
}

SoftProtocol::SoftProtocol() {

}

bool SoftProtocol::isError() { return m_error; }

QString SoftProtocol::errorString() { return m_errorString; }

// ==============================================================================================

/*void ISoftProtocolSubject::Attach(ISoftProtocolObserver* observer) {
    m_listeners.append(observer);
}

void ISoftProtocolSubject::Detach(ISoftProtocolObserver* observer) {
    m_listeners.removeAll(observer);
}

void ISoftProtocolSubject::Notify(quint8 addr, quint16 reg, quint16 value) {
    for(auto listener : m_listeners) {
        listener->update(addr, reg, value);
    }
}

void ISoftProtocolSubject::done() {
    for(auto listener : m_listeners) {
        listener->iamReady();
    }
}

void ISoftProtocolSubject::makeError(QString msg) {
    m_errorString = msg;
    for(auto listener : m_listeners) {
        listener->errorOccured(m_errorString);
    }

}*/
