#pragma once

#include <QObject>
#include <QVector>
#include <QWidget>

#include "interfaces/pubsubinterface.h"
#include "mainwindow.h"
#include "model/converterfactory.h"
#include "network/networkmodel.h"

class GuiFactory;
class MainWindow;
class Device;
class DeviceWidget;
class PlusMinusWidget;

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
  //  ConverterFactory m_converters;

 private slots:
  void createWidgetFor(Device* device);
  void createCalibAndLimitsWidgets(quint8 addr, quint16 id);
  void dataCapture(quint8 addr, quint16 code, quint16 value);

 signals:
  void deletedCalibrationDialog();
  void Signal_PublishEvent(model::Event);
};
