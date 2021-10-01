#include "connectionwidget.h"
#include "ui_connectionwidget.h"

ConnectionWidget::ConnectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionWidget)
{
    ui->setupUi(this);

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegularExpression ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegularExpressionValidator *ipValidator = new QRegularExpressionValidator(ipRegex, this);
    ui->NetworkIP->setValidator(ipValidator);
    QRegularExpression portRegex("^\\d\\d\\d\\d$");
    QRegularExpressionValidator *portValidator = new QRegularExpressionValidator(portRegex ,this);
    ui->NetworkPort->setValidator(portValidator);
    //connect(ui->NetworkConnect,&QPushButton::clicked,this, ConnectionWidget::setCurrentTcpPort());
    //ui->ConnectionTab->removeTab(ui->ConnectionTab->indexOf(ui->TCP));
}

ConnectionWidget::~ConnectionWidget()
{
    delete ui;
}

void ConnectionWidget::setConnectMessage(QString msg) {
    ui->NetworkStatus->setText(msg);
}

void ConnectionWidget::setConnected(bool isConnected) {
    qDebug() << "MainWindow::setConnected" << isConnected;
    if(isConnected) {
        ui->NetworkConnect->setText(tr("Disconnect"));
    } else {
        ui->NetworkConnect->setText(tr("Connect"));
    }
}

void ConnectionWidget::setBaudList(const QStringList& baudrateList){

    ui->Baudrate->insertItems(0,baudrateList);
}

void ConnectionWidget::setPortList(const QStringList& portList){
    ui->COMPort->insertItems(0,portList);
}

void ConnectionWidget::setCurrentComPort(QStringView port){
    ui->COMPort->setCurrentText(port.toString());
}

void ConnectionWidget::setCurrentIp(QStringView ip){
    ui->NetworkIP->setText(ip.toString());
}

void ConnectionWidget::setCurrentTcpPort(QStringView port){
    ui->NetworkPort->setText(port.toString());
}

void ConnectionWidget::setProtocol(NetworkType type){
    QVariantHash networkMap;
    networkMap.insert("type",  static_cast<quint8>(type));
    if (type == NetworkType::Tcp){
        networkMap.insert("host", ui->NetworkIP->text());
        networkMap.insert("port", ui->NetworkPort->text());
    }
    else if(type == NetworkType::SerialPort){
        networkMap.insert("comport", ui->COMPort->currentText());
        networkMap.insert("baudrate", ui->Baudrate->currentText());
    }
    else
        return;
    emit makeEvent("NetworkConnectClicked", networkMap);
}
