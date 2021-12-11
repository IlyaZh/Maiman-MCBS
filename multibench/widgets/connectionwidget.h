#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QWidget>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "network/networkmodel.h"
#include "constants.h"

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
    void setProtocol(PortType type);
    void setCurrentIp(QStringView ip);
    void setCurrentTcpPort(int port);

    void setConnectMessage(QStringView msg);
    void setConnected(bool isConnected);

private slots:
    void connectIsClicked(PortType type);

signals:
    void connectButtonClicked(PortType type, QVariant value);
    void refreshComPorts();

private:
    Ui::ConnectionWidget *ui;
    void paintEvent(QPaintEvent*) override;

};

#endif // CONNECTIONWIDGET_H
