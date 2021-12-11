#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QDebug>
#include "mainwindow.h"
#include "network/networkmodel.h"

class MainController :
        public QObject
{
    Q_OBJECT
public:
    explicit MainController(MainWindow& window, NetworkModel& networkModel, QObject *parent = nullptr);

private slots:
    void refreshComPorts();
    void connectToNetwork(QVariant value);
    void rescan();

signals:
    void rescanNetwork();
private:
    MainWindow& m_window;
    NetworkModel& m_network;
//    QPointer<DataSource> m_device;

};

#endif // MAINCONTROLLER_H
