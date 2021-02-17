#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <QObject>
#include "network/networkmodel.h"
#include "model/ModelCommandInterface.h"
#include "enums.h"

class MainWindowController : public QObject
{
    Q_OBJECT
public:
    explicit MainWindowController(QObject *parent = nullptr);
    void addModel(ModelInterface* model);
    void removeModel();

public slots:
    void networkConnectClicked(CONNECT_PROTOCOL protocol, QString host, int port);

signals:
    void connected(bool);

private:
    ModelInterface* m_model;
    ModelCommand* m_connectCommand;

};

#endif // MAINWINDOWCONTROLLER_H
