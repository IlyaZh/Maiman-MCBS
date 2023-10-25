#pragma once

#include <QVariant>
#include <QWidget>

#include "gui/guimediator.h"

using commandData = std::variant<int, double, QString>;

struct dataPackage {
  dataPackage(int code, double value, QString unit)
      : commandCode(code), valueDouble(value), valueUnit(unit){};
  int commandCode;
  double valueDouble;
  QString valueUnit;
};

class GuiInterface {
 public:
  explicit GuiInterface() = default;
  virtual ~GuiInterface() = default;
  virtual void getData(QSharedPointer<CommandConverter> data) = 0;
  virtual QVector<quint16> Subscribe() = 0;
 signals:

 public slots:
};
Q_DECLARE_INTERFACE(GuiInterface, "interfaces::Publisher");
