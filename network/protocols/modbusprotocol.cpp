#include "modbusprotocol.h"
#include <QDebug>
#include <QDateTime>

// ==============================================================================================
ModbusProtocol::ModbusProtocol(IDataSource* device, int timeout_ms, int delay_ms, QObject* parent) :
    QObject(parent),
    SoftProtocol(delay_ms),
    m_timeoutMs(timeout_ms)
{
    if(device != nullptr)
        setDevice(*device);
    m_timeoutTmr.setSingleShot(true);
    m_timeoutTmr.setInterval(timeout_ms);
    connect(&m_timeoutTmr, &QTimer::timeout, this, &ModbusProtocol::timeout_Slot);
}

ModbusProtocol::~ModbusProtocol() {

}

void ModbusProtocol::setDataValue(quint8 addr, quint16 reg, quint16 value) {
    QByteArray package = QByteArray();
    package.append(static_cast<qint8>(addr));
    package.append(static_cast<qint8>(WriteOne));
    package.append(static_cast<qint8>(hiBYTE(reg)));
    package.append(static_cast<qint8>(loBYTE(reg)));
    package.append(static_cast<qint8>(hiBYTE(value)));
    package.append(static_cast<qint8>(loBYTE(value)));

    prepareAndWrite(package);
}

void ModbusProtocol::getDataValue(quint8 addr, quint16 reg, quint8 count) {
    QByteArray package = QByteArray();
    package.append(static_cast<qint8>(addr));
    package.append(static_cast<qint8>(Read));
    package.append(static_cast<qint8>(hiBYTE(reg)));
    package.append(static_cast<qint8>(loBYTE(reg)));
    package.append(static_cast<qint8>(hiBYTE(count)));
    package.append(static_cast<qint8>(loBYTE(count)));

    prepareAndWrite(package);
}

// private methods
void ModbusProtocol::readyRead_Slot() {
    QByteArray rxPacket = m_device->readAll();
    m_timeoutTmr.stop();
    if(calcCrc(rxPacket) == 0) {
        rxPacketHandler(rxPacket);
    } else {
        m_errorString = QString("[%1] Wrong CRC. Ignore packet").arg("Modbus");
    }
    bPortIsBusy = false;

    tryToSend();
}

void ModbusProtocol::bytesWritten_Slot(qint64 bytes) {
    m_bytesWritten += bytes;
    if(m_bytesWritten >= m_lastTxPackage.size()) {
        if(m_lastTxPackage.at(0) == Broadcast) {
            tryToSend();
        } else {
            m_timeoutTmr.start(m_timeoutMs);
        }
    }
}

void ModbusProtocol::timeout_Slot() {
    bPortIsBusy = false;
//    qDebug() << "timeout() Set bPortIsBusy = " << bPortIsBusy;
    //    emit timeoutOccured(m_lastTxPackage->at(0));
//    qDebug() << "Timeout occured" << m_lastTxPackage.toHex(' ');

    switch(m_lastTxPackage.at(1)) {
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
        //        if(m_observer != nullptr)
        //        m_observer->timeout(m_lastTxPackage.at(0));
        break;
    case 0x05:
    case 0x06:
    case 0x10:
    case 0x0F:
        break;
    }

    tryToSend();
}

void ModbusProtocol::delayBeforeSend_Slot() {
    if(m_device.isNull()) {
        qDebug() << "m_device.isNull()";

    } else {
        if(!m_PriorityQueue.isEmpty()) {
            m_lastTxPackage = m_PriorityQueue.dequeue();
            //                qDebug() << "m_PriorityQueue size" << m_PriorityQueue.size();
        } else if (!m_Queue.isEmpty()) {
            m_lastTxPackage = m_Queue.dequeue();
            //        qDebug() << "m_Queue size" << m_Queue.size();
        } else {
            done();
            return;
        }

        m_lastWriteReg = (quint16) (m_lastTxPackage.at(2) << 8) | (quint16) (m_lastTxPackage.at(3));
        m_device->write(m_lastTxPackage);
        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "write: " << m_lastTxPackage.toHex(' ');
        m_timeoutTmr.start(m_timeoutMs);
    }
}

// end of private slots

quint16 ModbusProtocol::calcCrc(const QByteArray &byteArray) {
    quint16 RetCRC = 0xffff;	// CRC initialization
    quint8 i = 0;
    quint8 index = 0;

    while (i < byteArray.size()) {
        index = hiBYTE(RetCRC) ^ static_cast<quint8>(byteArray.at(i));
        RetCRC = static_cast<quint16> (((loBYTE(RetCRC) ^ CRC_HTable[index]) << 8)
                                       | (CRC_LTable[index]));
        i++;
    }
    return RetCRC;
}

void ModbusProtocol::rxPacketHandler(const QByteArray &rxPacket) {
    QString errorMessage = QString();
    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "RX" << rxPacket.toHex(' ');
    if(rxPacket.size() >= 6) {
        quint8 addr = static_cast<quint8>(rxPacket.at(0));
        int comm = rxPacket.at(1);
        int bytesCount = 0;
        quint16 reg = 0, value = 0;
        switch(comm) {
        case Read:
            bytesCount = rxPacket.at(2);
            if(rxPacket.size() == 5+bytesCount) {
                for(quint16 i = 0; i < bytesCount/2; ++i) {
                    reg = m_lastWriteReg+i;
                    value = static_cast<quint16>((static_cast<quint16>(rxPacket.at(3+2*i)) << 8) + static_cast<quint16>(rxPacket.at(4+2*i)));
                    Notify(addr, reg, value);
                }
            } else {
                errorMessage = QString("[%1} Wrong size of packet").arg("Modbus");
            }
            break;
        case WriteOne:
            reg = static_cast<quint16>((static_cast<quint16>(rxPacket.at(2)) << 8) + static_cast<quint16>(rxPacket.at(3)));
            value = static_cast<quint16>((static_cast<quint16>(rxPacket.at(4)) << 8) + static_cast<quint16>(rxPacket.at(5)));
            Notify(addr, reg, value);
            break;
        default:
            errorMessage = QString("[%1] Wrong command").arg("Modbus");
        }
    } else {
        errorMessage = QString("[%1] Wrong size of packet").arg("Modbus");
    }

    if(!errorMessage.isEmpty()) m_errorString = QString("%1 [%2]").arg(errorMessage).arg(QString().fromLocal8Bit(rxPacket));
}

void ModbusProtocol::prepareAndWrite(QByteArray &byteArray) {
    m_bytesWritten = 0;

    if(m_device->isOpen()) {
        quint16 crc = calcCrc(byteArray);
        byteArray.append(static_cast<qint8>(hiBYTE(crc)));
        byteArray.append(static_cast<qint8>(loBYTE(crc)));

        qDebug() << "prepareAndWrite()" << byteArray.toHex(' ');

        switch(byteArray.at(1)) {
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
            // read commands
            m_Queue.enqueue(byteArray);
            break;
        case 0x05:
        case 0x06:
        case 0x10:
        case 0x0F:
            m_PriorityQueue.enqueue(byteArray);
            // write commands
            break;
        }

        tryToSend();
    } else {
        m_errorString = QString("[%1] device isn't open!").arg("Modbus");
    }
}

const quint8 ModbusProtocol::CRC_HTable[256] = { 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
                                                 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
                                                 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
                                                 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
                                                 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
                                                 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
                                                 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
                                                 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80,
                                                 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
                                                 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
                                                 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
                                                 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
                                                 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
                                                 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
                                                 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
                                                 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
                                                 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
                                                 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
                                                 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
                                                 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
                                                 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
                                                 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 };

const quint8 ModbusProtocol::CRC_LTable[256] = { 0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02,
                                                 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D,
                                                 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08,
                                                 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF,
                                                 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16,
                                                 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31,
                                                 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34,
                                                 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B,
                                                 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A,
                                                 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25,
                                                 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20,
                                                 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7,
                                                 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E,
                                                 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9,
                                                 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC,
                                                 0x7C, 0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3,
                                                 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52,
                                                 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D,
                                                 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58,
                                                 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F,
                                                 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46,
                                                 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40
                                               };


// ==============================================================================================

//ModbusOld::ModbusOld(QString name, int timeoutMSecs, int delayMSecs, QObject *parent)
//    : SoftProtocolOld(name, timeoutMSecs, delayMSecs, parent)
//{
//    qDebug() << "Constr Set bPortIsBusy = " << bPortIsBusy;
//}

//ModbusOld::~ModbusOld() {

//}

//void ModbusOld::stop() {
//    m_Queue.clear();
//    m_PriorityQueue.clear();
//    bPortIsBusy = false;
//    qDebug() << "Modbus::stop(). Clear queues";
//    emit deviceOpen(m_DataSource->isOpen());
//}

//void ModbusOld::setDataValue(quint8 addr, quint16 reg, quint16 value) {
//    QByteArray package = QByteArray();
//    package.append(static_cast<qint8>(addr));
//    package.append(static_cast<qint8>(WRITE_ONE));
//    package.append(static_cast<qint8>(hiBYTE(reg)));
//    package.append(static_cast<qint8>(loBYTE(reg)));
//    package.append(static_cast<qint8>(hiBYTE(value)));
//    package.append(static_cast<qint8>(loBYTE(value)));

//    prepareAndWrite(package);
//}

//void ModbusOld::getDataValue(quint8 addr, quint16 reg, quint8 count) {
//    QByteArray package = QByteArray();
//    package.append(static_cast<qint8>(addr));
//    package.append(static_cast<qint8>(READ));
//    package.append(static_cast<qint8>(hiBYTE(reg)));
//    package.append(static_cast<qint8>(loBYTE(reg)));
//    package.append(static_cast<qint8>(hiBYTE(count)));
//    package.append(static_cast<qint8>(loBYTE(count)));

//    prepareAndWrite(package);
//}

//// private slots
//void ModbusOld::readyRead() {
//    QByteArray rxPacket = m_DataSource->readAll();
//    qDebug() << "Modbus::readyRead() ";
//    m_timeoutTimer.stop();
//    if(calcCrc(rxPacket) == 0) {
//        rxPacketHandler(rxPacket);
//    } else {
//        emit errorOccured(QString("[%1] Wrong CRC. Ignore packet").arg(m_name));
//    }
//    //    qDebug() << "rx packet: " << rxPacket;
//    bPortIsBusy = false;
//    qDebug() << "Set bPortIsBusy = " << bPortIsBusy;

//    tryToSend();
//}

//void ModbusOld::bytesWritten(qint64 bytes) {
//    m_bytesWritten += bytes;
//    if(m_bytesWritten >= m_lastTxPackage.size()) {
//        if(m_lastTxPackage.at(0) == BROADCAST) {
//            tryToSend();
//        } else {
//            m_timeoutTimer.start(m_TimeoutMSecs);
//            qDebug() << "start send timeout" << m_TimeoutMSecs;
//        }
//    }
//}

//void ModbusOld::timeout() {
//    bPortIsBusy = false;
//    qDebug() << "timeout() Set bPortIsBusy = " << bPortIsBusy;
//    //    emit timeoutOccured(m_lastTxPackage->at(0));
//    qDebug() << "Timeout occured" << m_lastTxPackage.toHex(' ');

//    switch(m_lastTxPackage.at(1)) {
//    case 0x01:
//    case 0x02:
//    case 0x03:
//    case 0x04:
//        if(m_observer != nullptr)
//            m_observer->timeout(m_lastTxPackage.at(0));
//        break;
//    case 0x05:
//    case 0x06:
//    case 0x10:
//    case 0x0F:
//        break;
//    }

//    tryToSend();
//}

//void ModbusOld::delayBeforeSendCallback() {
//    if(!m_PriorityQueue.isEmpty()) {
//        m_lastTxPackage = m_PriorityQueue.dequeue();
//        qDebug() << "m_PriorityQueue size" << m_PriorityQueue.size();
//    } else if (!m_Queue.isEmpty()) {
//        m_lastTxPackage = m_Queue.dequeue();
//        qDebug() << "m_Queue size" << m_Queue.size();
//    } else {
//        nothingToSend();
//        return;
//    }
//    qDebug() << "bPortIsBusy=" << bPortIsBusy;

//    //    if(!bPortIsBusy) {
//    m_lastWriteReg = (quint16) (m_lastTxPackage.at(2) << 8) | (quint16) (m_lastTxPackage.at(3));
//    SoftProtocolOld::m_DataSource->write(m_lastTxPackage);
//    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "write: " << m_lastTxPackage.toHex(' ');
//    m_timeoutTimer.start(m_TimeoutMSecs);
//    //    }
//}

//quint16 ModbusOld::calcCrc(QByteArray &byteArray) {
//    quint16 RetCRC = 0xffff;	// CRC initialization
//    quint8 i = 0;
//    quint8 index = 0;

//    while (i < byteArray.size()) {
//        index = hiBYTE(RetCRC) ^ static_cast<quint8>(byteArray.at(i));
//        RetCRC = static_cast<quint16> (((loBYTE(RetCRC) ^ CRC_HTable[index]) << 8)
//                                       | (CRC_LTable[index]));
//        i++;
//    }
//    return RetCRC;
//}

//void ModbusOld::prepareAndWrite(QByteArray &byteArray) {
//    m_bytesWritten = 0;

//    if(m_DataSource->isOpen()) {
//        quint16 crc = calcCrc(byteArray);
//        byteArray.append(static_cast<qint8>(hiBYTE(crc)));
//        byteArray.append(static_cast<qint8>(loBYTE(crc)));

//        switch(byteArray.at(1)) {
//        case 0x01:
//        case 0x02:
//        case 0x03:
//        case 0x04:
//            // read commands
//            m_Queue.enqueue(byteArray);
//            break;
//        case 0x05:
//        case 0x06:
//        case 0x10:
//        case 0x0F:
//            m_PriorityQueue.enqueue(byteArray);
//            // write commands
//            break;
//        }

//        tryToSend();
//    } else {
//        emit errorOccured(QString("[%1] device isn't open!").arg(m_name));
//    }
//}

//void ModbusOld::rxPacketHandler(QByteArray &rxPacket) {
//    QString errorMessage = "";
//    //    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "RX" << rxPacket.toHex(' ');
//    if(rxPacket.size() >= 6) {
//        quint8 addr = static_cast<quint8>(rxPacket.at(0));
//        int comm = rxPacket.at(1);
//        int bytesCount = 0;
//        quint16 reg = 0, value = 0;
//        switch(comm) {
//        case ModbusOld::READ:
//            bytesCount = rxPacket.at(2);
//            if(rxPacket.size() == 5+bytesCount) {
//                for(quint16 i = 0; i < bytesCount/2; ++i) {
//                    reg = m_lastWriteReg+i;
//                    value = static_cast<quint16>((static_cast<quint16>(rxPacket.at(3+2*i)) << 8) + static_cast<quint16>(rxPacket.at(4+2*i)));
//                    makeNotify(addr, reg, value);
//                }
//            } else {
//                errorMessage = QString("[%1} Wrong size of packet").arg(m_name);
//            }
//            break;
//        case ModbusOld::WRITE_ONE:
//            reg = static_cast<quint16>((static_cast<quint16>(rxPacket.at(2)) << 8) + static_cast<quint16>(rxPacket.at(3)));
//            value = static_cast<quint16>((static_cast<quint16>(rxPacket.at(4)) << 8) + static_cast<quint16>(rxPacket.at(5)));
//            makeNotify(addr, reg, value);
//            break;
//        default:
//            errorMessage = QString("[%1] Wrong command").arg(m_name);
//        }
//    } else {
//        errorMessage = QString("[%1] Wrong size of packet").arg(m_name);
//    }

//    if(!errorMessage.isEmpty()) emit errorOccured(QString("%1 [%2]").arg(errorMessage).arg(QString().fromLocal8Bit(rxPacket)));

//}

//void ModbusOld::tryToSend() {
//    qDebug() << "Try to send" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "bPortIsBusy=" << bPortIsBusy;
//    if(!bPortIsBusy) {
//        bPortIsBusy = true;
//        qDebug() << "Set bPortIsBusy = " << bPortIsBusy;
//        m_delayTimer.singleShot(m_delayMSecs, this, SLOT(delayBeforeSendCallback()));
//    }
//}


//const quint8 ModbusOld::CRC_HTable[256] = { 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//                                            0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//                                            0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//                                            0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
//                                            0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//                                            0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//                                            0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//                                            0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80,
//                                            0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//                                            0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//                                            0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//                                            0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
//                                            0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//                                            0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
//                                            0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
//                                            0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
//                                            0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//                                            0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//                                            0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//                                            0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
//                                            0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//                                            0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 };

//const quint8 ModbusOld::CRC_LTable[256] = { 0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02,
//                                            0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D,
//                                            0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08,
//                                            0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF,
//                                            0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16,
//                                            0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31,
//                                            0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34,
//                                            0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B,
//                                            0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A,
//                                            0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25,
//                                            0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20,
//                                            0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7,
//                                            0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E,
//                                            0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9,
//                                            0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC,
//                                            0x7C, 0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3,
//                                            0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52,
//                                            0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D,
//                                            0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58,
//                                            0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F,
//                                            0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46,
//                                            0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40
//                                          };


