#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "interfaces/mainwindowco1ntrollerinterface.h"
#include "model/device/devicewidget.h"
//class MainWindowControllerInterface;
//#include <QtWidgets>
#include "constants.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
//    static const QString SettingsPath;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void addDeviceWidget(DeviceWidget* widget);
    void setComPorts(const QStringList& portList);
    void setBaudRates(const QStringList& baudsList);

signals:
    void refreshComPortsSignal();
    void connectToNetwork(QVariant value);
    void tempratureUnitsChanged(Const::TemperatureUnitId id);
public slots:
    void setConnectMessage(QString msg);
    void setConnected(bool flag);
    void setStatusMessage(const QString& msg, int timeout = 10);
private slots:
    void adjust(const QSize& size = QSize());

private:
    Ui::MainWindow *ui;
//    MainWindowControllerInterface* m_cntrl;
//    QVector<QString> *m_portList;
//    QVector<int> *m_baudList;
    QVector<DeviceWidget*> m_workWidgets;
    QVBoxLayout* m_workFieldLayout;

protected:
    void closeEvent(QCloseEvent *event) override;

//    void setConnections();
};
#endif // MAINWINDOW_H
