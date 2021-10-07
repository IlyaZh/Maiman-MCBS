#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
//#include "interfaces/mainwindowco1ntrollerinterface.h"
#include "appsettings.h"
#include "model/device/devicewidget.h"
//class MainWindowControllerInterface;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const QString SettingsPath;

    MainWindow(AppSettings& settings, QWidget *parent = nullptr);
    ~MainWindow();
    void addDeviceWidget(DeviceWidget* widget);
    void setComPorts(const QStringList& portList);
    void setBaudRates(const QStringList& baudsList);

signals:
    void makeEvent(QString event, QVariant value = QVariant());
public slots:
    void setConnectMessage(QString msg);
    void setConnected(bool flag);
private slots:
    void adjust(const QSize& size = QSize());

private:
    Ui::MainWindow *ui;
//    MainWindowControllerInterface* m_cntrl;
    QVector<QString> *m_portList;
    QVector<int> *m_baudList;
    AppSettings& m_settings;
    QVector<DeviceWidget*> m_workWidgets;
    QGridLayout* m_workFieldLayout;

//    void setConnections();
};
#endif // MAINWINDOW_H
