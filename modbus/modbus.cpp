#include "modbus.h"

const quint8 Modbus::MODBUS_MAX_ADDRESS_CONST = 32;
const quint8 Modbus::MODBUS_TIMEOUT_DEFAULT = 100;

Modbus::Modbus(QIODevice* device, QString deviceName, int timeoutMSecs,  QObject *parent) : QObject(parent)
{
    setDevice(device, deviceName, timeoutMSecs);
    timeoutTimer.setSingleShot(true);
    connect(&timeoutTimer, &QTimer::timeout, [=]{

    });
}

Modbus::~Modbus() {
    closeDevice();
}

void Modbus::setDevice(QIODevice* device, QString deviceName, int timeoutMSecs) {
    closeDevice();

    if(m_Device) {
        disconnect(m_Device, nullptr, nullptr, nullptr);
    }
    m_Device = device;
    m_DeviceName = deviceName;
    setTimeout(timeoutMSecs);
    connect(m_Device, &QIODevice::readyRead, this, &Modbus::readyRead);
    connect(m_Device, &QIODevice::bytesWritten, this, &Modbus::bytesWritten);

    openDevice();
}

void Modbus::setTimeout(int MSecs) {
    m_TimeoutMSecs = MSecs;
}

void Modbus::setEnable(bool bNewState) {
    if(m_Device) {
        if(bNewState) {
            openDevice();
        } else {
            closeDevice();
        }
    }
}

bool Modbus::isEnable() {
    if(m_Device) {
        return m_Device->isOpen();
    } else {
        return false;
    }
}

void Modbus::addObserver(ModbusObserverInterface& newObserver) {
    m_ObserverVector.append(&newObserver);
}

void Modbus::setDataValue(quint8 addr, quint16 reg, quint16 value) {
    QByteArray* package = new QByteArray();
    package->append(static_cast<qint8>(addr));
    package->append(static_cast<qint8>(WRITE_ONE));
    package->append(static_cast<qint8>(hiBYTE(reg)));
    package->append(static_cast<qint8>(loBYTE(reg)));
    package->append(static_cast<qint8>(hiBYTE(value)));
    package->append(static_cast<qint8>(loBYTE(value)));

    m_lastWriteReg = reg;

    prepareToWrite(package);
}

void Modbus::getDataValue(quint8 addr, quint16 reg, quint8 count) {
    QByteArray* package = new QByteArray();
    package->append(static_cast<qint8>(addr));
    package->append(static_cast<qint8>(READ));
    package->append(static_cast<qint8>(hiBYTE(reg)));
    package->append(static_cast<qint8>(loBYTE(reg)));
    package->append(static_cast<qint8>(hiBYTE(count)));
    package->append(static_cast<qint8>(loBYTE(count)));

    m_lastWriteReg = reg;

    prepareToWrite(package);
}

// private slots
void Modbus::readyRead() {
    QByteArray rxPacket = m_Device->readAll();
    if(calcCrc(&rxPacket) == 0) {
        rxPacketHandler(&rxPacket);
    } else {
        emit errorOccured("Wrong CRC. Ignore packet");
        if(!m_Queue.isEmpty()) {
            m_lastTxPackage = m_Queue.dequeue();
            m_Device->write((*m_lastTxPackage));
        }
    }

}

void Modbus::bytesWritten(qint64 bytes) {
    m_bytesWritten += bytes;
    if(m_bytesWritten >= m_lastTxPackage->size()) {
        if(m_lastTxPackage->at(0) == 0) {
            if(!m_Queue.isEmpty()) {
                m_lastTxPackage = m_Queue.dequeue();
                m_Device->write((*m_lastTxPackage));
            }
        } else {
            timeoutTimer.start(m_TimeoutMSecs);
        }
    }
}

// private methods
bool Modbus::openDevice() {
    if(!m_Device->isOpen()) {
        if(!m_Device->open(QIODevice::ReadWrite)) {
            emit errorOccured("Can't open device");
        }
    }
    return m_Device->isOpen();
}

void Modbus::closeDevice() {
    if(m_Device) {
        if(m_Device->isOpen()) m_Device->close();
    }
}

quint8 Modbus::hiBYTE(quint16 value) {
    return ((value >> 8) & 0xff);
}

quint8 Modbus::loBYTE(quint16 value) {
    return (value & 0xff);
}

static const uint8_t CRC_HTable[256] = { 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
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

static const uint8_t CRC_LTable[256] = { 0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02,
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
        0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40 };

quint16 Modbus::calcCrc(QByteArray *byteArray) {
    quint16 RetCRC = 0xffff;	// CRC initialization
    quint8 i = 0;
    quint8 index = 0;

    while (i < byteArray->size()) {
        index = hiBYTE(RetCRC) ^ static_cast<quint8>(byteArray->at(i));
        RetCRC = static_cast<quint16> (((loBYTE(RetCRC) ^ CRC_HTable[index]) << 8)
                | (CRC_LTable[index]));
        i++;
    }
    return RetCRC;
}

void Modbus::prepareToWrite(QByteArray *byteArray) {
    m_bytesWritten = 0;

    if(m_Device->isOpen()) {
        quint16 crc = calcCrc(byteArray);
        byteArray->append(static_cast<qint8>(hiBYTE(crc)));
        byteArray->append(static_cast<qint8>(loBYTE(crc)));
        m_Queue.enqueue(byteArray);

        if(m_Queue.size() == 1) {
            m_lastTxPackage = m_Queue.dequeue();
            m_Device->write((*m_lastTxPackage));
        }
    } else {
        emit errorOccured(QString("Device [%1] isn't open!").arg(m_DeviceName));
    }
}

void Modbus::rxPacketHandler(QByteArray *rxPacket) {
    QString errorMessage = QString();
    if(rxPacket->size() >= 6) {
        quint8 addr = static_cast<quint8>(rxPacket->at(0));
        int comm = rxPacket->at(1);
        int bytesCount = 0;
        quint16 reg = 0, value = 0;
        switch(comm) {
            case Modbus::READ:
            bytesCount = rxPacket->at(2);
            if(rxPacket->size() == 6+bytesCount) {
                for(quint16 i = 0; i < bytesCount/2; ++i) {
                    reg = m_lastWriteReg+i;
                    value = static_cast<quint16>((static_cast<quint16>(rxPacket->at(4+2*i)) << 8) + static_cast<quint16>(rxPacket->at(5+2*i)));
                    makeNotify(addr, reg, value);
                }
            } else {
                errorMessage = "Wrong size of packet";
            }
            break;
        case Modbus::WRITE_ONE:
             reg = static_cast<quint16>((static_cast<quint16>(rxPacket->at(2)) << 8) + static_cast<quint16>(rxPacket->at(3)));
             value = static_cast<quint16>((static_cast<quint16>(rxPacket->at(4)) << 8) + static_cast<quint16>(rxPacket->at(5)));
             makeNotify(addr, reg, value);
            break;
        default:
            errorMessage = "Wrong command";
        }
    } else {
        errorMessage = "Wrong size of packet";
    }

    if(!errorMessage.isEmpty()) emit errorOccured(QString("%1 [%2]").arg(errorMessage).arg(QString().fromLocal8Bit(*rxPacket)));

}

void Modbus::makeNotify(quint8 addr, quint16 reg, quint16 value) {
    for(ModbusObserverInterface* item : m_ObserverVector) {
        item->notify(addr, reg, value);
    }
}
