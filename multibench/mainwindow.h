#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "constants.h"

class AboutDialog;
class UpdateWidget;
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

public slots:
    void setConnectMessage(QString msg);
    void setConnected(bool flag);
    void setStatusMessage(const QString& msg, int timeout = 10);
    void updateDownloadingfinished();
    void triggeredRescanNetwork();
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
protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
