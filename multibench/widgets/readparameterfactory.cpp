#include "readparameterfactory.h"
#include "model/device/devicewidget.h"
#include "device/commandsettings.h"

ReadParameterFactory::ReadParameterFactory()
{

}

ReadParameterWidget* ReadParameterFactory::createReadParameter(QStringView name, QSharedPointer<DevCommand> cmd){
    if(cmd->code() == SerialCommand){
        auto widget = new ReadNoUnitParameterWidget();
        widget->setup(name, cmd);
        return widget;
    }
    else {
        auto widget = new ReadParameterWidget();
        widget->setup(name, cmd);
        return widget;
    }
}
