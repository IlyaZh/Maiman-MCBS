#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QWidget>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
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

//public slots:
    void setBaudList(const QStringList& baudrateList);
    void setPortList(const QStringList& portList);
    void setCurrentComPort(QStringView port);
    void setProtocol(NetworkType type);
    void setCurrentIp(QStringView ip);
    void setCurrentTcpPort(QStringView port);

    void setConnectMessage(QString msg);
    void setConnected(bool isConnected);

private slots:
    void connectedIsClicked(NetworkType type);

signals:
    void deviceIsConnected(QVariant EventValue);
    void refreshComPorts();

private:
    Ui::ConnectionWidget *ui;
};

#endif // CONNECTIONWIDGET_H
