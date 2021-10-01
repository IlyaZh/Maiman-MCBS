#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QWidget>
#include "network/networkmodel.h"

namespace Ui {
class ConnectionWidget;
}

class ConnectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionWidget(QWidget *parent = nullptr);
    ~ConnectionWidget();


public slots:
    void setBaudList(const QStringList& baudrateList);
    void setPortList(const QStringList& portList);
    void setCurrentComPort(QStringView port);
    void setProtocol(NetworkType type);
    void setCurrentIp(QStringView ip);
    void setCurrentTcpPort(QStringView port);

    void setConnectMessage(QString msg);
    void setConnected(bool isConnected);
private:
    Ui::ConnectionWidget *ui;
    const QString baudrate;
};

#endif // CONNECTIONWIDGET_H
