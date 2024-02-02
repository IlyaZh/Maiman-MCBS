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
class DeviceWidget;
class PlusMinusWidget;
class CalibrationDialog;

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
  QMap<quint8, QSharedPointer<CalibrationDialog>> m_calibrationDialog;
  QMap<quint8, QPointer<DeviceWidget>> m_deviceWidgetsTable;
  QMap<int, QPointer<GroupWidget>> m_groupWidgetsTable;

 private slots:
  void createWidgetFor(Device* device);
  void createCalibAndLimitsWidgets(quint8 addr, quint16 id);
  void createGroupManagerWidget();
  void dataCapture(quint8 addr, quint16 code, quint16 value);
  void createGroupWidgetFor(const QSet<quint8>& addresses, int number);
  void deleteGroupWidgetFor(int address);
  void modifMemberGroup(bool isRemove, int groupAddr, quint8 devAddr);

 signals:
  void deletedCalibrationDialog();
  void Signal_PublishEvent(model::Event);
  void finishGroupAction();
};
