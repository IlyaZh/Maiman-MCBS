#include "readparameterfactory.h"

ReadParameterFactory::ReadParameterFactory()
{

}

ReadParameterWidget* ReadParameterFactory::createReadParameter(QStringView name, QSharedPointer<DevCommand> cmd){
    auto widget = new ReadParameterWidget();
    if(name.toString() == "Serial Number")
        widget = new ReadNoUnitParameterWidget();
    widget->setup(name, cmd);
    return widget;
}
