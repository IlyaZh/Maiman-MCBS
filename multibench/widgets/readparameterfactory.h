#ifndef READPARAMETERFACTORY_H
#define READPARAMETERFACTORY_H

#include <QObject>

#include "widgets/readparameterwidget.h"
#include "widgets/readnounitparameterwidget.h"

enum { SerialCommand = 0x3 };

class ReadParameterFactory : public QObject
{
        Q_OBJECT
public:
    ReadParameterFactory();
    static ReadParameterWidget* createReadParameter(QStringView name, QSharedPointer<DevCommand> cmd);
};

#endif // READPARAMETERFACTORY_H
