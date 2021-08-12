#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
//#include "interfaces/mainwindowco1ntrollerinterface.h"
#include "appsettings.h"
//class MainWindowControllerInterface;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    const QString SETTINGS_PATH = "window/";

    MainWindow(AppSettings& settings, QWidget *parent = nullptr);
    ~MainWindow();
//    void addController(MainWindowControllerInterface* cntrl);
//    void clearController();
    void setNetworkMessage(QString msg);
    void setPortList(QVector<QString> *portList);
    void setBaudList(QVector<int> *baudList);
//    void addSettingsCntrl(AppSettings* settings);
    void addToWorkField(QWidget* widget);

signals:
    void makeEvent(QString event, QVariant value);
//    void networkConnectClicked(NetworkType type, QVariant host, QVariant port);
//    void networkConnectClicked(int, QString, int);
    void mainWindowReady();
public slots:
    void setConnectMessage(QString msg);
    void setConnected(bool flag);
private slots:
    void on_networkConnectButton_clicked();

private:
    Ui::MainWindow *ui;
//    MainWindowControllerInterface* m_cntrl;
    QVector<QString> *m_portList;
    QVector<int> *m_baudList;
    AppSettings& m_settings;
    QVector<QWidget*> m_workWidgets;
    QGridLayout* m_workFieldLayout;

    void setConnections();
};
#endif // MAINWINDOW_H
