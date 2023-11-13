#ifndef READNOUNITPARAMETERWIDGET_H
#define READNOUNITPARAMETERWIDGET_H

#include "widgets/readparameterwidget.h"
struct Control;
class DevCommand;

class ReadNoUnitParameterWidget : public ReadParameterWidget {
 public:
  ReadNoUnitParameterWidget();
  void setup(QStringView name, QSharedPointer<DevCommand> cmd) override;
  void setup(QStringView name, QSharedPointer<CommandConverter> cmd) override;
};

#endif  // READNOUNITPARAMETERWIDGET_H
