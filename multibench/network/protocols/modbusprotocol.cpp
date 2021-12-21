#include "modbusprotocol.h"
#include <QDateTime>

// ==============================================================================================
quint16 ModbusProtocol::calcCrc(const QByteArray &byteArray) {
    quint16 RetCRC = 0xffff;	// CRC initialization
    quint8 i = 0;

    while (i < byteArray.size()) {
        quint8 index = hiBYTE(RetCRC) ^ static_cast<quint8>(byteArray.at(i));
        RetCRC = static_cast<quint16> (((loBYTE(RetCRC) ^ CRC_HTable.at(index)) << 8)
                                       | (CRC_LTable.at(index)));
        i++;
    }
    return RetCRC;
}

ModbusProtocol::ModbusProtocol(QObject* parent) : QObject(parent) {}

QByteArray ModbusProtocol::setDataValue(quint8 addr, quint16 reg, quint16 value) {
    QByteArray package = QByteArray();
    package.append(static_cast<quint8>(addr));
    package.append(static_cast<quint8>(WriteOne));
    package.append(static_cast<quint8>(hiBYTE(reg)));
    package.append(static_cast<quint8>(loBYTE(reg)));
    package.append(static_cast<quint8>(hiBYTE(value)));
    package.append(static_cast<quint8>(loBYTE(value)));
    quint16 crc = calcCrc(package);
    package.append(static_cast<quint8>(hiBYTE(crc)));
    package.append(static_cast<quint8>(loBYTE(crc)));
    return package;
}

QByteArray ModbusProtocol::getDataValue(quint8 addr, quint16 reg, quint8 count) {
    QByteArray package = QByteArray();
    package.append(static_cast<quint8>(addr));
    package.append(static_cast<quint8>(Read));
    package.append(static_cast<quint8>(hiBYTE(reg)));
    package.append(static_cast<quint8>(loBYTE(reg)));
    package.append(static_cast<quint8>(hiBYTE(count)));
    package.append(static_cast<quint8>(loBYTE(count)));
    quint16 crc = calcCrc(package);
    package.append(static_cast<quint8>(hiBYTE(crc)));
    package.append(static_cast<quint8>(loBYTE(crc)));
    return package;
}

ModbusProtocol::DataVector ModbusProtocol::execute(const QByteArray& rxPackage, const QByteArray& lastTxPackage) {
    m_error = false;
    m_errorString.clear();
    DataVector resultVector;
    if(rxPackage.size() >= 6) {
        if(calcCrc(rxPackage) == 0) {
            quint8 addr = static_cast<quint8>(rxPackage.at(0));
            quint8 comm = static_cast<quint8>(rxPackage.at(1));
            int bytesCount = 0;
            quint16 reg = 0;
            quint16 value = 0;
            switch(comm) {
            case Read:
                bytesCount = rxPackage.at(2);
                if(rxPackage.size() == 5+bytesCount) {
                    if(lastTxPackage.size() >= 4) {
                        quint16 lastWriteReg = ((static_cast<quint16>(lastTxPackage.at(2)) << 8)&0xff00) + static_cast<quint8>(lastTxPackage.at(3));
                        for(quint16 i = 0; i < bytesCount/2; ++i) {
                            reg = lastWriteReg+i;
                            value = static_cast<quint16>(((static_cast<quint16>(rxPackage.at(3+2*i)) << 8)&0xff00) + static_cast<quint8>(rxPackage.at(4+2*i)));
                            SoftProtocolData dataUnit;
                            dataUnit.addr = addr;
                            dataUnit.reg = reg;
                            dataUnit.value = value;
                            resultVector.append(dataUnit);
                        }
                    }
                } else {
                    makeError(QString("[%1} Wrong size of packet").arg("Modbus"));
                }
                break;
            case WriteOne:
                reg = static_cast<quint16>((static_cast<quint16>(rxPackage.at(2)) << 8) + static_cast<quint16>(rxPackage.at(3)));
                value = static_cast<quint16>(((static_cast<quint16>(rxPackage.at(4)) << 8)&0xff00) + static_cast<quint8>(rxPackage.at(5)));
                resultVector.append({addr, reg, value});
                break;
            default:
                makeError(QString("[%1] Wrong command").arg("Modbus"));
            }

        } else {
            makeError(u"Checksum error");
        }
    } else {
        makeError(QString("[%1] Wrong size of packet").arg("Modbus"));
    }
    return resultVector;
}

bool ModbusProtocol::needWaitForAnswer(const QByteArray& package) {
    if(package.size() > 2) {
        quint8 addr = package.at(0);
        quint8 cmd = package.at(1);
        switch(cmd) {
        case WriteOne:
            if(addr == Broadcast) {
                return false;
            }
            break;
        }
        return true;
    }
    return false;
}

qint64 ModbusProtocol::waitForBytes(const QByteArray& package) {
    if(package.size() >= 6) {
        quint8 addr = static_cast<quint8>(package.at(0));
        quint8 cmd = static_cast<quint8>(package.at(1));
        quint16 count = 0;
        switch(cmd) {
        case WriteOne:
            if(addr == Broadcast) {
                return 0;
            }
            return 8;
            break;
        case Read:
            count = (static_cast<quint8>(package.at(4)) << 8) + static_cast<quint8>(package.at(5));
            return 5+2*count;
            break;
        }
    }
    return 0;
}

void ModbusProtocol::makeError(QStringView msg) {
    m_error = true;
    m_errorString = msg.toString();
}


const QVector<quint8> ModbusProtocol::CRC_HTable { 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
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

const QVector<quint8> ModbusProtocol::CRC_LTable { 0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02,
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
