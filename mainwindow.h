#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainwindowcontroller.h"
#include "appsettings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    const QString SETTINGS_PATH = "window/";

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addController(MainWindowController& cntrl);
    void clearController();
    void setNetworkMessage(QString msg);
    void setPortList(QVector<QString> *portList);
    void setBaudList(QVector<int> *baudList);
//    void addSettingsCntrl(AppSettings* settings);

signals:
    void networkConnectButtonSignal();
    void mainWindowReady();

private slots:
    void setConnected(bool flag);


    void on_networkConnectButton_clicked();

private:
    Ui::MainWindow *ui;
    MainWindowController* m_cntrl;
    QVector<QString> *m_portList;
    QVector<int> *m_baudList;
    AppSettings* m_settings;

    void setConnections();
};
#endif // MAINWINDOW_H
