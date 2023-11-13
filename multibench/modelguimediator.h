#ifndef MODELGUIMEDIATOR_H
#define MODELGUIMEDIATOR_H

#include <QObject>
#include <QVector>
#include <QWidget>

#include "mainwindow.h"
#include "model/converterfactory.h"
#include "network/networkmodel.h"

class GuiFactory;
class MainWindow;
class Device;
class DeviceWidget;
class PlusMinusWidget;

class ModelGuiMediator : public QObject {
  Q_OBJECT
 public:
  explicit ModelGuiMediator(MainWindow& window, GuiFactory& factory,
                            NetworkModel& networkModel,
                            QObject* parent = nullptr);

 private:
  MainWindow& m_window;
  GuiFactory& m_factory;
  NetworkModel& m_network;
  QMap<quint8, quint16> m_calibrationDialog;
  ConverterFactory* m_converters;

 private slots:
  void createWidgetFor(Device* device);
  void createCalibAndLimitsWidgets(quint8 addr, quint16 id);
 private slots:
  void refreshComPorts();
  void changeConnectState(Const::PortType type, QVariantMap value);
  void rescan();
  void setBaudrateToWindow(QStringList baud);
 signals:
  void rescanNetwork();
  void deletedCalibrationDialog();
};

#endif  // MODELGUIMEDIATOR_H
