#pragma once

#include <QObject>
//#include <QIODevice>
#include "datasource.h"
#include <QPointer>
#include <QTimer>
#include <QQueue>
#include <QVector>
#include "datasource.h"
#include "interfaces/ProtocolObserverInterface.h"

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

    SoftProtocol();
    virtual ~SoftProtocol(){}
    virtual QByteArray setDataValue(quint8 addr, quint16 reg, quint16 value) = 0;
    virtual QByteArray getDataValue(quint8 addr, quint16 reg, quint8 count = 1) = 0;
    virtual DataVector execute(const QByteArray& rxPackage, const QByteArray& lastTxPackage) = 0;
    virtual bool needWaitForAnswer(const QByteArray& package) = 0;
    bool isError();
    QString errorString();

protected:
    QString m_errorString;
    bool m_error = false;
};


/*class ISoftProtocolObserver {
public:
    virtual ~ISoftProtocolObserver(){};
    virtual void update(quint8 addr, quint16 reg, quint16 value) = 0;
    virtual void iamReady() = 0;
    virtual void errorOccured(const QString& msg) = 0;
};

class ISoftProtocolSubject {
public:
    ~ISoftProtocolSubject(){};
    virtual void Attach(ISoftProtocolObserver* observer);
    virtual void Detach(ISoftProtocolObserver* observer);
protected:
    virtual void Notify(quint8 addr, quint16 reg, quint16 value);
    virtual void done();
    virtual void makeError(QString msg);
protected:
    QList<ISoftProtocolObserver*> m_listeners;
    QString m_errorString = "";
};*/

