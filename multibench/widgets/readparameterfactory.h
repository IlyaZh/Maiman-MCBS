#ifndef READPARAMETERFACTORY_H
#define READPARAMETERFACTORY_H

#include <QObject>

#include "widgets/readnounitparameterwidget.h"
#include "widgets/readparameterwidget.h"

class ReadParameterFactory : public QObject {
  Q_OBJECT
 public:
  ReadParameterFactory();
  static ReadParameterWidget* createReadParameter(
      QStringView name, QSharedPointer<DevCommand> cmd);
  static ReadParameterWidget* createReadParameter(
      QStringView name, QSharedPointer<CommandConverter> cmd);
};

#endif  // READPARAMETERFACTORY_H
