#ifndef MODELGUIMEDIATOR_H
#define MODELGUIMEDIATOR_H

#include <QObject>
#include <QVector>
#include <QWidget>

#include "mainwindow.h"
#include "network/networkmodel.h"

class GuiFactory;
class MainWindow;

class NetworkMediator : public QObject {
  Q_OBJECT
 public:
  explicit NetworkMediator(MainWindow& window, GuiFactory& factory,
                           NetworkModel& networkModel,
                           QObject* parent = nullptr);

 private:
  MainWindow& m_window;
  GuiFactory& m_factory;
  NetworkModel& m_network;

 private slots:
  void refreshComPorts();
  void changeConnectState(Const::PortType type, QVariantMap value);
  void rescan();
  void setBaudrateToWindow(QStringList baud);
 signals:
  void rescanNetwork();
};

#endif  // MODELGUIMEDIATOR_H
