#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "constants.h"

class AboutDialog;
class UpdateWidget;
// class CalibrationMenu;
class PlusMinusWidget;
class DeviceWidget;
class RescanProgressWidget;
class QuitDialog;
class ConnectionWidget;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}  // namespace Ui
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;
  void addDeviceWidget(DeviceWidget* widget);
  void setComPorts(const QStringList& portList);
  void setBaudRates(const QStringList& baudsList);

 signals:
  void refreshComPortsSignal();
  void changeConnectState(Const::PortType type, QVariantMap value);
  void tempratureUnitsChanged(Const::TemperatureUnitId id);
  void rescanNetwork();
  void createCalibAndLimitsWidgets(quint8 addr, quint16 id);
  void delayChanged(int delay);
  void timeoutChanged(int timeout);

 public slots:
  void setConnectMessage(QString msg);
  void setConnected(bool flag);
  void setStatusMessage(const QString& msg,
                        int timeout = Const::kStatusbarMessageTimeout);
  void triggeredRescanNetwork();
  void addCalibrationMenu(quint8 addr, quint16 id);
  void rescanProgress(int current, int total, int success);
  void slot_serialPortClosed(const QString& msg);
 private slots:
  void comTriggered();
  void tcpTriggered();
  void callAboutDialog();
  void getKeepAddresses();
  void setNetworkDelay();
  void setNetworkTimeout();
  void setNetworkMaxAddress();
  void deviceNameChanged(QString name, int addr);

  void setBothComPorts(QString port);
  void setBothBaudRates(QString baudtare);

 private:
  Ui::MainWindow* ui;
  QVector<DeviceWidget*> m_workWidgets;
  QGridLayout* m_workFieldLayout;
  QPointer<QActionGroup> m_portGroup;
  QPointer<QActionGroup> m_baudrateGroup;
  UpdateWidget* m_updater;
  QPointer<AboutDialog> m_About;
  QPointer<QuitDialog> m_Quit;
  QPointer<RescanProgressWidget> m_progressWidget;
  QPointer<ConnectionWidget> m_connectionWidget;
  bool m_isConnected{false};

 protected:
  void closeEvent(QCloseEvent* event) override;
};
#endif  // MAINWINDOW_H
