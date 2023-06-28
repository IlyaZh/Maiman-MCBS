#include "readparameterfactory.h"

#include "device/commandsettings.h"
#include "model/device/devicewidget.h"

enum { SerialCommand = 0x3 };

ReadParameterFactory::ReadParameterFactory() {}

ReadParameterWidget* ReadParameterFactory::createReadParameter(
    QStringView name, QSharedPointer<DevCommand> cmd) {
  ReadParameterWidget* widget;

  if (cmd->code() == SerialCommand) {
    widget = new ReadNoUnitParameterWidget();
  } else {
    widget = new ReadParameterWidget();
  }

  widget->setup(name, cmd);
  return widget;
}
