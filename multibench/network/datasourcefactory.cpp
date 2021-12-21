#include "datasourcefactory.h"
#include "tcpdatasource.h"
#include "serialdatasource.h"

DataSourceFactory::DataSourceFactory(QObject *parent)
    : QObject{parent}
{

}

IDataSource* DataSourceFactory::createSource(PortType type) {
    switch(type) {
    case PortType::Com:
        return new SerialDataSource;
    case PortType::TCP:
        return new TcpDataSource;
    default:
        return nullptr;
    }
}
