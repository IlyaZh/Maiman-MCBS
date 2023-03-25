#include "datasourcefactory.h"

#include "serialdatasource.h"
#include "tcpdatasource.h"

DataSourceFactory::DataSourceFactory(QObject* parent) : QObject{parent} {}

IDataSource* DataSourceFactory::createSource(Const::PortType type) {
  switch (type) {
    case Const::PortType::kCom:
      return new SerialDataSource;
    case Const::PortType::kTCP:
      return new TcpDataSource;
    default:
      return nullptr;
  }
}
