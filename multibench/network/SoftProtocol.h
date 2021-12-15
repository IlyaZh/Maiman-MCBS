#pragma once

#include <QObject>
//#include <QIODevice>
#include "datasource.h"
#include <QPointer>
#include <QTimer>
#include <QQueue>
#include <QVector>
#include "datasource.h"
//#include "interfaces/ProtocolObserverInterface.h"

struct SoftProtocolData {
    quint8 addr;
    quint16 reg;
    quint16 value;

    bool operator==(const SoftProtocolData& other) const {
        return ((addr == other.addr)
                && (reg == other.reg)
                && (value == other.value));
    }
};

class SoftProtocol {
public:
    using DataVector = QVector<SoftProtocolData>;
    static const quint8 MaxAddress;
    static quint8 hiBYTE(quint16 value);
    static quint8 loBYTE(quint16 value);

    SoftProtocol() = default;
    virtual ~SoftProtocol() = default;
    SoftProtocol(SoftProtocol&&) = default;
    SoftProtocol& operator=(SoftProtocol&&) = default;

    virtual QByteArray setDataValue(quint8 addr, quint16 reg, quint16 value) = 0;
    virtual QByteArray getDataValue(quint8 addr, quint16 reg, quint8 count = 1) = 0;
    virtual DataVector execute(const QByteArray& rxPackage, const QByteArray& lastTxPackage) = 0;
    virtual bool needWaitForAnswer(const QByteArray& package) = 0;
    virtual qint64 waitForBytes(const QByteArray& package) = 0;
    bool isError();
    QString errorString();

protected:
    QString m_errorString;
    bool m_error = false;
};
