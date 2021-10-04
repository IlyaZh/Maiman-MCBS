#include "connectionwidget.h"
#include "ui_connectionwidget.h"

ConnectionWidget::ConnectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionWidget)
{
    ui->setupUi(this);
    ui->networkIpLineEdit->setPlaceholderText("127.0.0.1");
    ui->networkPortLineEdit->setPlaceholderText("500");
    ui->comPortComboBox->setPlaceholderText("COM4");
    ui->baudrateComboBox->setPlaceholderText("115200");

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegularExpression ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegularExpressionValidator *ipValidator = new QRegularExpressionValidator(ipRegex, this);
    ui->networkIpLineEdit->setValidator(ipValidator);

    connect(ui->networkConnectButton, &QPushButton::clicked, this, [this](){
     connectedIsClicked(NetworkType::Tcp);
    });
    connect(ui->connectComPortButton, &QPushButton::clicked, this, [this](){
     connectedIsClicked(NetworkType::SerialPort);
    });
    connect(ui->refreshComPortButton, &QPushButton::clicked, this, &ConnectionWidget::refreshComPorts);
}

ConnectionWidget::~ConnectionWidget()
{
    delete ui;
}

void ConnectionWidget::setConnectMessage(QString msg) {
    ui->networkStatusLabel->setText(msg);
    ui->statusComPortLabel->setText(msg);
}

void ConnectionWidget::setConnected(bool isConnected) {
    qDebug() << "MainWindow::setConnected" << isConnected;
    if(isConnected) {
        ui->networkConnectButton->setText(tr("Disconnect"));
    } else {
        ui->networkConnectButton->setText(tr("Connect"));
    }
}

void ConnectionWidget::setBaudList(const QStringList& baudrateList){

    ui->baudrateComboBox->addItems(baudrateList);
}

void ConnectionWidget::setPortList(const QStringList& portList){
    ui->comPortComboBox->addItems(portList);
}

void ConnectionWidget::setCurrentComPort(QStringView port){
    ui->comPortComboBox->setCurrentText(port.toString());
}

void ConnectionWidget::setCurrentIp(QStringView ip){
    ui->networkIpLineEdit->setText(ip.toString());
}

void ConnectionWidget::setCurrentTcpPort(QStringView port){
    ui->networkPortLineEdit->setText(port.toString());
}

void ConnectionWidget::setProtocol(NetworkType type){

    if (type == NetworkType::Tcp){
        ui->ConnectionTab->setCurrentWidget(ui->TcpTab);
    }
    else if(type == NetworkType::SerialPort){
        ui->ConnectionTab->setCurrentWidget(ui->ComTab);
    }
    else
        return;
}

void ConnectionWidget::connectedIsClicked(NetworkType type){
    QVariantHash networkMap;
    networkMap.insert("type",  static_cast<quint8>(type));
    if (type == NetworkType::Tcp){
        networkMap.insert("host", ui->networkIpLineEdit->text());
        networkMap.insert("port", ui->networkPortLineEdit->text());
    }
    else if(type == NetworkType::SerialPort){
        networkMap.insert("comport", ui->comPortComboBox->currentText());
        networkMap.insert("baudrate", ui->baudrateComboBox->currentText());
    }
    else
        return;
    emit deviceClicked(networkMap);
}
