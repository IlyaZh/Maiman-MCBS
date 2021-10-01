#include "connectionwidget.h"
#include "ui_connectionwidget.h"

ConnectionWidget::ConnectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionWidget)
{
    ui->setupUi(this);
    //ui->ConnectTab->removeTab(ui->ConnectTab->indexOf(ui->TCP));
}

ConnectionWidget::~ConnectionWidget()
{
    delete ui;
}

void ConnectionWidget::setConnectMessage(QString msg) {
    ui->NetworkStatus->setText(msg);;
}

void ConnectionWidget::setConnected(bool isConnected) {
    qDebug() << "MainWindow::setConnected" << isConnected;
    if(isConnected) {
        ui->NetworkConnect->setText(tr("Disconnect"));
    } else {
        ui->NetworkConnect->setText(tr("Connect"));
    }
}
