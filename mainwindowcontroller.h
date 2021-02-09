#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <QObject>
#include "network/networkmodel.h"

class MainWindowController : public QObject
{
    Q_OBJECT
public:
    enum CONNECT_PROTOCOL { UNKNOWN_PROTOCOL, TCP_PROTOCOL, COM_PORT_PROTOCOL };
    explicit MainWindowController(QObject *parent = nullptr);
    void addModel(NetworkModelInInterface* model);
    void removeModel();

public slots:
    void networkConnectClicked(CONNECT_PROTOCOL protocol, QString host, int port);

signals:
    void connected(bool);

private:
    NetworkModelInInterface* m_model;

};

#endif // MAINWINDOWCONTROLLER_H
