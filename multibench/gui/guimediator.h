#pragma once

#include <QObject>
#include <QVector>
#include <QWidget>

#include "interfaces/pubsubinterface.h"
#include "mainwindow.h"
#include "network/networkmodel.h"
#include "widgets/groupmanager.h"

class GuiFactory;
class MainWindow;
class Device;
class DeviceHolder;
class PlusMinusWidget;
class CalibrationDialog;
struct groupCheckBoxes;

class GuiMediator : public QObject, public interfaces::Subscriber {
  Q_OBJECT
 public:
  explicit GuiMediator(MainWindow& window, GuiFactory& factory,
                       NetworkModel& networkModel, QObject* parent = nullptr);

  void NewEvent(const model::Event& event) override;
 public slots:
  void clear();

 private:
  MainWindow& m_window;
  GuiFactory& m_factory;
  NetworkModel& m_network;
  QMap<quint8, QSharedPointer<CalibrationDialog>> m_calibrationDialog;
  QMap<quint8, QPointer<DeviceHolder>> m_deviceWidgetsTable;
  QMap<int, QPointer<GroupWidget>> m_groupWidgetsTable;

 private slots:
  void createWidgetFor(Device* device);
  void createCalibAndLimitsWidgets(quint8 addr, quint16 id);
  void createGroupManagerWidget();
  void dataCapture(quint8 addr, quint16 code, quint16 value);
  void deleteGroupWidgetFor(int address);
  void recreateGroups(const QMap<int, QSharedPointer<groupCheckBoxes>>& groups);

 signals:
  void deletedCalibrationDialog();
  void Signal_PublishEvent(model::Event);
  void repaintGroupsAndDevices();
};
