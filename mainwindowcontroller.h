#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <QObject>
#include "network/networkmodel.h"

class MainWindowController : public QObject
{
    Q_OBJECT
public:
    explicit MainWindowController(QObject *parent = nullptr);
    void addModel(ModelInterface* model);
    void removeModel();

public slots:
    void networkConnectClicked(ModelInterface::CONNECT_PROTOCOL protocol, QString host, int port);

signals:
    void connected(bool);

private:
    ModelInterface* m_model;

};

#endif // MAINWINDOWCONTROLLER_H
