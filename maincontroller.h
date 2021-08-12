#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QDebug>
#include "appsettings.h"
#include "mainwindow.h"
#include "network/networkmodel.h"

class MainController :
        public QObject
{
    Q_OBJECT
public:
    explicit MainController(MainWindow& window, NetworkModel& networkModel, AppSettings& settings, QObject *parent = nullptr);

private slots:
    void eventHandle(const QString &event, const QVariant &value);

signals:

private:
    MainWindow& window;
    NetworkModel& network;
    AppSettings& settings;
    QPointer<DataSource> device;

};

#endif // MAINCONTROLLER_H
