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
    if(!bPortIsBusy) {
        bPortIsBusy = true;
        m_delayTmr.singleShot(m_delayMs, [this](){
            delayBeforeSend_Slot();
        });
    }
}
// ================================

//const quint8 SoftProtocolOld::MAX_ADDRESS = 32;
//const int SoftProtocolOld::TIMEOUT_DEFAULT = 100;
//const int SoftProtocolOld::DELAY_DEFAULT = 50;

//SoftProtocolOld::SoftProtocolOld(QString name, int timeoutMSecs, int delayMSecs, QObject *parent) : QObject(parent), m_DataSource(nullptr) {
//    m_name = name;
//    m_delayMSecs = delayMSecs;
//    setTimeout(timeoutMSecs);
//    m_timeoutTimer.setSingleShot(true);
//    m_delayTimer.setSingleShot(true);
//    connect(&m_timeoutTimer, SIGNAL(timeout()), this, SLOT(timeout()));
//    bPortIsBusy = false;
//    qDebug() << "[softprotocol constr] Set bPortIsBusy = " << bPortIsBusy;
//}

//SoftProtocolOld::~SoftProtocolOld() {
//    //    if(m_DataSource != nullptr) m_DataSource->deleteLater();
//}

//void SoftProtocolOld::start(IDataSource& device/*, int timeoutMSecs*/) {

//    m_DataSource = &device;

//    connect(m_DataSource, SIGNAL(readyRead()), this, SLOT(readyRead()));
//    connect(m_DataSource, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
//    emit deviceOpen(true);

//    //    openDevice();
//}

//void SoftProtocolOld::setTimeout(int MSecs) {
//    m_TimeoutMSecs = MSecs;
//    m_timeoutTimer.setInterval(m_TimeoutMSecs);
//}

//void SoftProtocolOld::setDelay(int MSecs) {
//    m_delayMSecs = MSecs;
//    m_delayTimer.setInterval(m_delayMSecs);
//}

//bool SoftProtocolOld::isEnable() {
//    if(m_DataSource) {
//        return m_DataSource->isOpen();
//    } else {
//        return false;
//    }
//}

//void SoftProtocolOld::addObserver(ProtocolObserverInterface* newObserver) {
//    m_observer = newObserver;
//    qDebug() << "m_observer = newObserver;";
//}

//void SoftProtocolOld::removeObserver(ProtocolObserverInterface* pObserver) {
//    m_observer = nullptr;
//    qDebug() << "m_observer = nullptr;";
//}

//void SoftProtocolOld::setDataValue(quint8 addr, quint16 reg, quint16 value) {
//    QByteArray package = QByteArray();
//    package.append(static_cast<qint8>(addr));
//    package.append(static_cast<qint8>(hiBYTE(reg)));
//    package.append(static_cast<qint8>(loBYTE(reg)));
//    package.append(static_cast<qint8>(hiBYTE(value)));
//    package.append(static_cast<qint8>(loBYTE(value)));

//    prepareAndWrite(package);
//}


//void SoftProtocolOld::getDataValue(quint8 addr, quint16 reg, quint8 count) {
//    QByteArray package;
//    package.append(static_cast<qint8>(addr));
//    package.append(static_cast<qint8>(hiBYTE(reg)));
//    package.append(static_cast<qint8>(loBYTE(reg)));
//    package.append(static_cast<qint8>(hiBYTE(count)));
//    package.append(static_cast<qint8>(loBYTE(count)));

//    prepareAndWrite(package);
//}

//void SoftProtocolOld::stop() {
//    m_Queue.clear();
//    qDebug() << "Clear queue";
//    //    if(m_DataSource != nullptr) {
//    //        m_DataSource->close();
//    //        m_DataSource->disconnect();
//    //        m_DataSource.clear();
//    emit deviceOpen(m_DataSource->isOpen());
//    qDebug() << "delete qiodevice";
//    //    }
//}

//// private slots
//void SoftProtocolOld::readyRead() {
//    QByteArray rxPacket = m_DataSource->readAll();
//    m_timeoutTimer.stop();
//    rxPacketHandler(rxPacket);
//    bPortIsBusy = false;
//    qDebug() << "[Softprotocol readyRead] Set bPortIsBusy = " << bPortIsBusy;

//    tryToSend();
//}

//void SoftProtocolOld::bytesWritten(qint64 bytes) {
//    m_bytesWritten += bytes;
//    if(m_bytesWritten >= m_lastTxPackage.size()) {
//        qDebug() << "m_TimeoutMSecs" << m_TimeoutMSecs;
//        m_timeoutTimer.start(m_TimeoutMSecs);
//    }
//}

//quint8 SoftProtocolOld::hiBYTE(quint16 value) {
//    return ((value >> 8) & 0xff);
//}

//quint8 SoftProtocolOld::loBYTE(quint16 value) {
//    return (value & 0xff);
//}

//quint16 SoftProtocolOld::calcCrc(QByteArray &byteArray) {
//    return 0; // 0 is means "OK"
//}

//void SoftProtocolOld::prepareAndWrite(QByteArray &byteArray) {
//    m_bytesWritten = 0;

//    if(m_DataSource->isOpen()) {
//        m_Queue.enqueue(byteArray);
//        tryToSend();
//    } else {
//        emit errorOccured(QString("[%1] device isn't open!").arg(m_name));
//    }
//}

//void SoftProtocolOld::rxPacketHandler(QByteArray &rxPacket) {
//    qDebug() << "[" << m_name << "] packet recevied: " << rxPacket << "\n";

//}

//void SoftProtocolOld::makeNotify(quint8 addr, quint16 reg, quint16 value) {
//    qDebug() << "makeNotify" << m_observer;
//    if(m_observer != nullptr)
//        m_observer->dataIncome(addr, reg, value); // доделай
//}

//void SoftProtocolOld::nothingToSend() {
//    if(m_observer != nullptr)
//        m_observer->dataReady();
//}

//void SoftProtocolOld::tryToSend() {
//    m_delayTimer.singleShot(m_delayMSecs, this, SLOT(delayBeforeSendCallback()));
//    //    delayTimeout();
//}

///*void SoftProtocol::timeout() {
//    bPortIsBusy = false;
////    emit timeoutOccured(m_lastTxPackage->at(0));

//    for(auto *obsrv : m_vObservers) {
//        obsrv->timeout(m_lastTxPackage->at(0));
//    }

//    tryToSend();
//}*/

//void SoftProtocolOld::delayBeforeSendCallback() {
//    if(m_Queue.isEmpty()) {
//        nothingToSend();
//    } else if(!bPortIsBusy) {
//        m_lastTxPackage = m_Queue.dequeue();
//        m_DataSource->write(m_lastTxPackage);
//        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "SoftProtocol write: " << m_lastTxPackage.toHex(' ');
//        m_timeoutTimer.start(m_TimeoutMSecs);
//        bPortIsBusy = true;
//        qDebug() << "[SoftProtocol::delayBeforeSendCallback()] Set bPortIsBusy = " << bPortIsBusy;
//    }
//}
