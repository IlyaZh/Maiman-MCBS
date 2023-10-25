#pragma once

#include <QObject>
#include <QVector>
#include <QWidget>

#include "interfaces/pubsubinterface.h"
#include "mainwindow.h"
#include "network/networkmodel.h"

class CommandSettings;
class GuiFactory;
class MainWindow;
class Device;
class DeviceWidget;
class PlusMinusWidget;

struct CommandConverter {
 public:
  explicit CommandConverter(const CommandSettings& conf, quint16 value);
  void changeTemperatureUnit(Const::TemperatureUnitId id);
  double convertCelToFar();
  double convertFarToCel();
  quint16 code();
  QString unit() const;
  double divider() const;
  int tolerance() const;
  bool isSigned() const;
  bool isTemperature() const;
  double valueDouble() const;
  uint valueInt() const;
  QString valueStr() const;
  uint interval() const;

 private:
  const CommandSettings& m_config;
  quint16 m_rawValue = 0;
  double m_value = 0;
  Const::TemperatureUnitId m_tempId{Const::TemperatureUnitId::kCelsius};
};

class GuiMediator : public QObject, public interfaces::Subscriber {
  Q_OBJECT
 public:
  explicit GuiMediator(MainWindow& window, GuiFactory& factory,
                       NetworkModel& networkModel, QObject* parent = nullptr);

  void NewEvent(const model::Event& event) override;

 private:
  MainWindow& m_window;
  GuiFactory& m_factory;
  NetworkModel& m_network;
  QMap<quint8, quint16> m_calibrationDialog;
  QHash<quint8, QPointer<DeviceWidget>> m_deviceWidgetsTable;

 private slots:
  void createWidgetFor(Device* device);
  void createCalibAndLimitsWidgets(quint8 addr, quint16 id);

 signals:
  void deletedCalibrationDialog();
};
