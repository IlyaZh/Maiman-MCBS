#include "SoftProtocol.h"
#include <QDebug>

const quint8 SoftProtocol::MAX_ADDRESS = 32;
const int SoftProtocol::TIMEOUT_DEFAULT = 100;

SoftProtocol::SoftProtocol(QString name, QIODevice* device, int timeoutMSecs, QObject *parent) : QObject(parent), m_Device(nullptr) {
    m_name = name;
    setDevice(device, timeoutMSecs);
    timeoutTimer.setSingleShot(true);
    connect(&timeoutTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    bPortIsBusy = false;
}

SoftProtocol::~SoftProtocol() {
    if(m_Device != nullptr) m_Device->deleteLater();
}

void SoftProtocol::setDevice(QIODevice* device, int timeoutMSecs) {
//    closeDevice();
    if(m_Device) {
        m_Device->disconnect();
        delete m_Device;
        m_Device = nullptr;
    }

    if(device != nullptr) {
        m_Device = device;
        setTimeout(timeoutMSecs);
        connect(m_Device, &QIODevice::readyRead, this, &SoftProtocol::readyRead);
        connect(m_Device, &QIODevice::bytesWritten, this, &SoftProtocol::bytesWritten);
    }

//    openDevice();
}

void SoftProtocol::setTimeout(int MSecs) {
    m_TimeoutMSecs = MSecs;
    timeoutTimer.setInterval(m_TimeoutMSecs);
}

bool SoftProtocol::isEnable() {
    if(m_Device) {
        return m_Device->isOpen();
    } else {
        return false;
    }
}

void SoftProtocol::addObserver(ProtocolObserverInterface* newObserver) {
    m_vObservers.append(newObserver);
}

void SoftProtocol::removeObserver(ProtocolObserverInterface* pObserver) {
    m_vObservers.removeOne(pObserver);
}

void SoftProtocol::setDataValue(quint8 addr, quint16 reg, quint16 value) {
    QByteArray* package = new QByteArray();
    package->append(static_cast<qint8>(addr));
    package->append(static_cast<qint8>(hiBYTE(reg)));
    package->append(static_cast<qint8>(loBYTE(reg)));
    package->append(static_cast<qint8>(hiBYTE(value)));
    package->append(static_cast<qint8>(loBYTE(value)));

    prepareAndWrite(package);
}

void SoftProtocol::getDataValue(quint8 addr, quint16 reg, quint8 count) {
    QByteArray* package = new QByteArray();
    package->append(static_cast<qint8>(addr));
    package->append(static_cast<qint8>(hiBYTE(reg)));
    package->append(static_cast<qint8>(loBYTE(reg)));
    package->append(static_cast<qint8>(hiBYTE(count)));
    package->append(static_cast<qint8>(loBYTE(count)));

    prepareAndWrite(package);
}

void SoftProtocol::stop() {
    if(m_Device != nullptr) {
        m_Device->disconnect();
        delete m_Device;
        m_Device = nullptr;
    }
    m_Queue.clear();
}

// private slots
void SoftProtocol::readyRead() {
    QByteArray rxPacket = m_Device->readAll();
    timeoutTimer.stop();
    rxPacketHandler(&rxPacket);
    bPortIsBusy = false;

    tryToSend();
}

void SoftProtocol::bytesWritten(qint64 bytes) {
    m_bytesWritten += bytes;
    if(m_bytesWritten >= m_lastTxPackage->size()) {
        timeoutTimer.start(m_TimeoutMSecs);
    }
}

quint8 SoftProtocol::hiBYTE(quint16 value) {
    return ((value >> 8) & 0xff);
}

quint8 SoftProtocol::loBYTE(quint16 value) {
    return (value & 0xff);
}

quint16 SoftProtocol::calcCrc(QByteArray *byteArray) {
    return 0; // 0 is means "OK"
}

void SoftProtocol::prepareAndWrite(QByteArray *byteArray) {
    m_bytesWritten = 0;

    if(m_Device == nullptr) {
        qDebug() << "[" << m_name << "] device pointer is nullptr";
        return;
    }

    if(m_Device->isOpen()) {
        m_Queue.enqueue(byteArray);
        tryToSend();
    } else {
        emit errorOccured(QString("[%] device isn't open!").arg(m_name));
    }
}

void SoftProtocol::rxPacketHandler(QByteArray *rxPacket) {
    qDebug() << "[" << m_name << "] packet recevied: " << rxPacket << "\n";

}

void SoftProtocol::makeNotify(quint8 addr, quint16 reg, quint16 value) {
    for(ProtocolObserverInterface* item : m_vObservers) {
        item->dataIncome(addr, reg, value);
    }
//    qDebug() << m_name << " rx: " << addr << reg << value;
}

void SoftProtocol::nothingToSend() {
    for(ProtocolObserverInterface* item : m_vObservers) {
        item->dataReady();
    }
}

void SoftProtocol::tryToSend() {
    if(m_Queue.isEmpty()) {
        nothingToSend();
    } else if(!bPortIsBusy) {
        m_lastTxPackage = m_Queue.dequeue();
        m_Device->write((*m_lastTxPackage));
        timeoutTimer.start(m_TimeoutMSecs);
        bPortIsBusy = true;

    }
}

void SoftProtocol::timeout() {
    bPortIsBusy = false;
    emit timeoutOccured();
    tryToSend();
}
