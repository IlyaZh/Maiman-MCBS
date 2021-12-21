#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model/device/devicewidget.h"
#include "constants.h"

class AboutDialog;
class UpdateWidget;
class CalibrationMenu;
class CalibrationAndLimitsWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void addDeviceWidget(DeviceWidget* widget);
    void setComPorts(const QStringList& portList);
    void setBaudRates(const QStringList& baudsList);
    void setStatusBarMessage(QString message);

signals:
    void refreshComPortsSignal();
    void changeConnectState(PortType type, QVariantMap value);
    void tempratureUnitsChanged(Const::TemperatureUnitId id);
    void rescanNetwork();
    void createCalibAndLimitsWidgets(quint8 addr, quint16 id);
    void finishEditedNetworkTimeout(quint16 timeout);

public slots:
    void setConnectMessage(QString msg);
    void setConnected(bool flag);
    void setStatusMessage(const QString& msg, int timeout = 10);
    void triggeredRescanNetwork();
    void addCalibrationMenu(quint8 addr,quint16 id);
private slots:
    void connectTriggered();
    void callAboutDialog();
    void getKeepAddresses();
    void setNetworkTimeout();
private:
    Ui::MainWindow *ui;
    QVector<DeviceWidget*> m_workWidgets;
    QGridLayout* m_workFieldLayout;
    QPointer<QActionGroup> m_portGroup;
    QPointer<QActionGroup> m_baudrateGroup;
    UpdateWidget* m_updater;
    QPointer<AboutDialog> m_About;
    QVector<CalibrationMenu*> m_calibrationDialogs;
protected:
    void closeEvent(QCloseEvent *event) override;

};
#endif // MAINWINDOW_H