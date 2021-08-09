#include "SoftProtocol.h"
#include <QDebug>
#include <QDateTime>

// ==============================================================================================

void ISoftProtocolSubject::Attach(ISoftProtocolObserver* observer) {
    m_listeners.append(observer);
}

void ISoftProtocolSubject::Detach(ISoftProtocolObserver* observer) {
    m_listeners.removeAll(observer);
}

void ISoftProtocolSubject::Notify(quint8 addr, quint16 reg, quint16 value) {
    for(auto it = m_listeners.begin(); it != m_listeners.end(); it++) {
        (*it)->update(addr, reg, value);
    }
}

void ISoftProtocolSubject::done() {
    for(auto it = m_listeners.begin(); it != m_listeners.end(); it++) {
        (*it)->iamReady();
    }
}

// ==============================================================================================

const quint8 SoftProtocol::MaxAddress = 32;
const int SoftProtocol::TimeoutDefault = 100;
const int SoftProtocol::DelayDefault = 50;

SoftProtocol::SoftProtocol(int delayMs) :
    m_delayMs(delayMs)
{

}

void SoftProtocol::setDevice(IDataSource &device) {
    if(m_device) m_device->disconnect();
    bPortIsBusy = false;
    qDebug() << "Clear portIsBusy";
    m_device = &device;
    m_device->connect(m_device, &IDataSource::readyRead, [this](){
        this->readyRead_Slot();
    });
    m_device->connect(m_device, &IDataSource::bytesWritten, [this](qint64 bytes){
        this->bytesWritten_Slot(bytes);
    });
}

QString SoftProtocol::errorString() {
    QString cpy = m_errorString;
    m_errorString.clear();
    return cpy;
}

SoftProtocol::State SoftProtocol::state() {
    return m_state;
}

quint8 SoftProtocol::hiBYTE(quint16 value) {
    return ((value >> 8) & 0xff);
}

quint8 SoftProtocol::loBYTE(quint16 value) {
    return (value & 0xff);
}

// private methods
void SoftProtocol::tryToSend() {
    qDebug() << "TryToSend busy=" << bPortIsBusy;
    if(!bPortIsBusy) {
        bPortIsBusy = true;
        qDebug() << "SET PORT BUSY";
        m_delayTmr.singleShot(m_delayMs, [this](){
            delayBeforeSend_Slot();
        });
    }
}
