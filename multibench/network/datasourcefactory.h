#pragma once

#include <QObject>

#include "constants.h"

class IDataSource;

class DataSourceFactory : public QObject {
  Q_OBJECT
  explicit DataSourceFactory(QObject* parent = nullptr);

 public:
  static IDataSource* createSource(Const::PortType type);
};
