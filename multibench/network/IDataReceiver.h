#ifndef IDATARECEIVER_H
#define IDATARECEIVER_H

#include <QtCore>

class IDataReceiver
{
public:
    virtual void connected()= 0;
    virtual void timeout(QScopedPointer<QByteArray>& lastPackage) = 0;
    virtual void readyRead() = 0;

};

#endif // IDATARECEIVER_H
