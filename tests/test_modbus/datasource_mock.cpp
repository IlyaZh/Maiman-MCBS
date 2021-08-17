#include "datasource_mock.h"
#include "../../network/SoftProtocol.h"
#include "../../network/protocols/modbusprotocol.h"
#include <QVector>

DataSource_Mock::DataSource_Mock(uint deviceCount, int sendDelay) : deviceCount(deviceCount), sendDelay(sendDelay)
{
    for(uint i = 0; i < deviceCount; i++)
        m_values.insert(i+1, QVector<quint16>(100,0));
}

void DataSource_Mock::setSettings(NetworkType type, QVariant host, QVariant port) {
    // nothing to do
}

bool DataSource_Mock::open() {
    m_isOpen = true;
    emit deviceOpen(m_isOpen);
    return m_isOpen;
}


void DataSource_Mock::close() {
    m_isOpen = false;
    emit deviceOpen(m_isOpen);
}

bool DataSource_Mock::isOpen() {
    return m_isOpen;
}

/*qint64 DataSource_Mock::write(const char* data, qint64 maxSize) {

}

qint64 DataSource_Mock::write(const char* data) {

}*/

qint64 DataSource_Mock::write(const QByteArray &byteArray) {
    m_answer.clear();
    emit bytesWritten(byteArray.size());
    int needCount = 6;
    if(byteArray.size() >= needCount) {
        quint8 addr = byteArray.at(0);
        quint8 cmd = byteArray.at(1);
        quint16 reg = (byteArray.at(2) << 8) + byteArray.at(3);
        quint16 regCount = 0;
        switch(cmd) {
        case 0x03:
            regCount = (byteArray.at(4) << 8) + byteArray.at(5);
            needCount = 6+regCount*2;
            break;
        case 0x06:
            regCount = 1;
            needCount = 6+regCount*2;
            break;
        }


        if(byteArray.size() == needCount) {
            int size = byteArray.size()-1;
            quint16 value;
            quint16 crc = (byteArray.at(size-1) << 8) + byteArray.at(size);
            quint8 errorCode = 0;
            QByteArray answData;
            if(crc == ModbusProtocol::calcCrc(byteArray)) {
                switch (cmd) {
                case 0x03:
                    if(m_values.contains(addr)) {
                        auto vec = m_values.value(addr);
                        if(reg+regCount <= vec.size()) {
                            answData.append(static_cast<quint8>(SoftProtocol::hiBYTE(reg)));
                            answData.append(static_cast<quint8>(SoftProtocol::loBYTE(reg)));
                            for(int i = reg; i < reg+regCount; ++i) {
                                quint16 value = vec.at(i);
                                answData.append(static_cast<quint8>(SoftProtocol::hiBYTE(value)));
                                answData.append(static_cast<quint8>(SoftProtocol::loBYTE(value)));
                            }
                        } else {
                            errorCode = 3;
                        }
                    } else {
                        errorCode = 2;
                    }
                    break;
                case 0x06:
                    if(m_values.contains(addr)) {
                        auto vec = m_values.value(addr);
                        if(reg+regCount <= vec.size()) {
                            value = (byteArray.at(4) << 8)+  byteArray.at(5);
                            answData.append(static_cast<quint8>(SoftProtocol::hiBYTE(reg)));
                            answData.append(static_cast<quint8>(SoftProtocol::loBYTE(reg)));
                            answData.append(static_cast<quint8>(SoftProtocol::hiBYTE(value)));
                            answData.append(static_cast<quint8>(SoftProtocol::loBYTE(value)));
                            vec[reg] = value;
                            m_values[addr] = vec;
                        } else {
                            errorCode = 3;
                        }
                    } else {
                        errorCode = 2;
                    }
                    break;
                default:
                    errorCode = 1;
                }

                if(errorCode > 0) {
                    m_answer.append(static_cast<quint8>(addr));
                    m_answer.append(static_cast<quint8>(cmd|0x80));
                    m_answer.append(static_cast<quint8>(errorCode));
                } else if(!answData.isEmpty()) {
                    m_answer.append(static_cast<quint8>(addr));
                    m_answer.append(static_cast<quint8>(cmd));
                    m_answer.append(answData);
                }

                if(!m_answer.isEmpty()) {
                    quint16 crc = ModbusProtocol::calcCrc(m_answer);
                    m_answer.append(static_cast<quint8>(SoftProtocol::hiBYTE(crc)));
                    m_answer.append(static_cast<quint8>(SoftProtocol::loBYTE(crc)));
                    timer.singleShot(sendDelay, [=]() {
                        emit readyRead();
                    });
                }

            } else {
                m_errorString = "Ошибка контрольной суммы";
                emit errorOccured(m_errorString);
            }
        }
    }

    return byteArray.size();
}

qint64 DataSource_Mock::read(char* data, qint64 maxSize) {
    qint64 size = (m_answer.size() > maxSize) ? maxSize : m_answer.size();
    for(qint64 i = 0; i < size; ++i) {
        data[i] = m_answer.at(i);
    }
    return size;
}

QByteArray DataSource_Mock::read(qint64 maxSize) {
    if(maxSize >= m_answer.size()) {
        return m_answer;
    } else {
        QByteArray tmp = m_answer;
        tmp.truncate(maxSize);
        return tmp;
    }
}

QByteArray DataSource_Mock::readAll() {
    return m_answer;
}

QString DataSource_Mock::errorString() {
    return m_errorString;
}
