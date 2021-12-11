#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "constants.h"

class AboutDialog;
class UpdateWidget;
class CalibrateDialog;
class CalibrationAndLimitsWidget;
class DeviceWidget;

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
    void connectToNetwork(QVariant value);
    void tempratureUnitsChanged(Const::TemperatureUnitId id);
    void rescanNetwork();
    void createCalibAndLimitsWidgets(quint8 addr);

public slots:
    void setConnectMessage(QString msg);
    void setConnected(bool flag);
    void setStatusMessage(const QString& msg, int timeout = 10);
    void triggeredRescanNetwork();
    void addCalibrationDialog(quint16 id, QVector<CalibrationAndLimitsWidget*> calibrations,QVector<CalibrationAndLimitsWidget*> limits);
private slots:
    void connectTriggered();
    void callAboutDialog();
    void getKeepAddresses();
private:
    Ui::MainWindow *ui;
    QVector<DeviceWidget*> m_workWidgets;
    QGridLayout* m_workFieldLayout;
    QPointer<QActionGroup> m_portGroup;
    QPointer<QActionGroup> m_baudrateGroup;
    UpdateWidget* m_updater;
    QPointer<AboutDialog> m_About;
    QVector<CalibrateDialog*> m_calibrationDialogs;
protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
