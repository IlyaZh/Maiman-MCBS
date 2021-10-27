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

signals:

private:
    MainWindow& window;
    NetworkModel& network;
    QPointer<DataSource> device;

};

#endif // MAINCONTROLLER_H
