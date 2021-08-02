#include "SoftProtocol.h"
#include <QDebug>
#include <QDateTime>

const quint8 SoftProtocol::MAX_ADDRESS = 32;
const int SoftProtocol::TIMEOUT_DEFAULT = 100;
const int SoftProtocol::DELAY_DEFAULT = 50;

SoftProtocol::SoftProtocol(QString name, int timeoutMSecs, int delayMSecs, QObject *parent) : QObject(parent), m_DataSource(nullptr) {
    m_name = name;
    m_delayMSecs = delayMSecs;
    setTimeout(timeoutMSecs);
    m_timeoutTimer.setSingleShot(true);
    m_delayTimer.setSingleShot(true);
    connect(&m_timeoutTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    bPortIsBusy = false;
    qDebug() << "[softprotocol constr] Set bPortIsBusy = " << bPortIsBusy;
}

SoftProtocol::~SoftProtocol() {
//    if(m_DataSource != nullptr) m_DataSource->deleteLater();
}

void SoftProtocol::start(DataSourceInterface& device/*, int timeoutMSecs*/) {

    m_DataSource = &device;

    connect(m_DataSource, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(m_DataSource, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    emit deviceOpen(true);

//    openDevice();
}

void SoftProtocol::setTimeout(int MSecs) {
    m_TimeoutMSecs = MSecs;
    m_timeoutTimer.setInterval(m_TimeoutMSecs);
}

void SoftProtocol::setDelay(int MSecs) {
    m_delayMSecs = MSecs;
    m_delayTimer.setInterval(m_delayMSecs);
}

bool SoftProtocol::isEnable() {
    if(m_DataSource) {
        return m_DataSource->isOpen();
    } else {
        return false;
    }
}

void SoftProtocol::addObserver(ProtocolObserverInterface* newObserver) {
    m_observer = newObserver;
    qDebug() << "m_observer = newObserver;";
}

void SoftProtocol::removeObserver(ProtocolObserverInterface* pObserver) {
    m_observer = nullptr;
    qDebug() << "m_observer = nullptr;";
}

void SoftProtocol::setDataValue(quint8 addr, quint16 reg, quint16 value) {
    QByteArray package = QByteArray();
    package.append(static_cast<qint8>(addr));
    package.append(static_cast<qint8>(hiBYTE(reg)));
    package.append(static_cast<qint8>(loBYTE(reg)));
    package.append(static_cast<qint8>(hiBYTE(value)));
    package.append(static_cast<qint8>(loBYTE(value)));

    prepareAndWrite(package);
}


void SoftProtocol::getDataValue(quint8 addr, quint16 reg, quint8 count) {
    QByteArray package;
    package.append(static_cast<qint8>(addr));
    package.append(static_cast<qint8>(hiBYTE(reg)));
    package.append(static_cast<qint8>(loBYTE(reg)));
    package.append(static_cast<qint8>(hiBYTE(count)));
    package.append(static_cast<qint8>(loBYTE(count)));

    prepareAndWrite(package);
}

void SoftProtocol::stop() {
    m_Queue.clear();
    qDebug() << "Clear queue";
//    if(m_DataSource != nullptr) {
//        m_DataSource->close();
//        m_DataSource->disconnect();
//        m_DataSource.clear();
        emit deviceOpen(m_DataSource->isOpen());
        qDebug() << "delete qiodevice";
//    }
}

// private slots
void SoftProtocol::readyRead() {
    QByteArray rxPacket = m_DataSource->readAll();
    m_timeoutTimer.stop();
    rxPacketHandler(rxPacket);
    bPortIsBusy = false;
    qDebug() << "[Softprotocol readyRead] Set bPortIsBusy = " << bPortIsBusy;

    tryToSend();
}

void SoftProtocol::bytesWritten(qint64 bytes) {
    m_bytesWritten += bytes;
    if(m_bytesWritten >= m_lastTxPackage.size()) {
        qDebug() << "m_TimeoutMSecs" << m_TimeoutMSecs;
        m_timeoutTimer.start(m_TimeoutMSecs);
    }
}

quint8 SoftProtocol::hiBYTE(quint16 value) {
    return ((value >> 8) & 0xff);
}

quint8 SoftProtocol::loBYTE(quint16 value) {
    return (value & 0xff);
}

quint16 SoftProtocol::calcCrc(QByteArray &byteArray) {
    return 0; // 0 is means "OK"
}

void SoftProtocol::prepareAndWrite(QByteArray &byteArray) {
    m_bytesWritten = 0;

    if(m_DataSource->isOpen()) {
        m_Queue.enqueue(byteArray);
        tryToSend();
    } else {
        emit errorOccured(QString("[%1] device isn't open!").arg(m_name));
    }
}

void SoftProtocol::rxPacketHandler(QByteArray &rxPacket) {
    qDebug() << "[" << m_name << "] packet recevied: " << rxPacket << "\n";

}

void SoftProtocol::makeNotify(quint8 addr, quint16 reg, quint16 value) {
    qDebug() << "makeNotify" << m_observer;
    if(m_observer != nullptr)
        m_observer->dataIncome(addr, reg, value); // доделай
}

void SoftProtocol::nothingToSend() {
    if(m_observer != nullptr)
        m_observer->dataReady();
}

void SoftProtocol::tryToSend() {
    m_delayTimer.singleShot(m_delayMSecs, this, SLOT(delayBeforeSendCallback()));
//    delayTimeout();
}

/*void SoftProtocol::timeout() {
    bPortIsBusy = false;
//    emit timeoutOccured(m_lastTxPackage->at(0));

    for(auto *obsrv : m_vObservers) {
        obsrv->timeout(m_lastTxPackage->at(0));
    }

    tryToSend();
}*/

void SoftProtocol::delayBeforeSendCallback() {
    if(m_Queue.isEmpty()) {
        nothingToSend();
    } else if(!bPortIsBusy) {
        m_lastTxPackage = m_Queue.dequeue();
        m_DataSource->write(m_lastTxPackage);
        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "SoftProtocol write: " << m_lastTxPackage.toHex(' ');
        m_timeoutTimer.start(m_TimeoutMSecs);
        bPortIsBusy = true;
        qDebug() << "[SoftProtocol::delayBeforeSendCallback()] Set bPortIsBusy = " << bPortIsBusy;
    }
}
