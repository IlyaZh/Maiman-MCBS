#include "connectionwidget.h"
#include "ui_connectionwidget.h"

#include <QPainter>

ConnectionWidget::ConnectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionWidget)
{
    ui->setupUi(this);
    ui->networkIpLineEdit->setPlaceholderText("127.0.0.1");
    ui->networkPortLineEdit->setPlaceholderText("500");
    //ui->comPortComboBox->setPlaceholderText("COM4");
    //ui->baudrateComboBox->setPlaceholderText("115200");

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegularExpression ipRegex ("^" + ipRange
                                + "\\." + ipRange
                                + "\\." + ipRange
                                + "\\." + ipRange + "$");
    QRegularExpressionValidator *ipValidator = new QRegularExpressionValidator(ipRegex, this);
    ui->networkIpLineEdit->setValidator(ipValidator);

    connect(ui->networkConnectButton, &QPushButton::clicked, this, [this](){
        connectClicked(PortType::TCP);
    });
    connect(ui->connectComPortButton, &QPushButton::clicked, this, [this](){
        connectClicked(PortType::Com);
    });
    connect(ui->refreshComPortButton, &QPushButton::clicked, this, &ConnectionWidget::refreshComPorts);
}

ConnectionWidget::~ConnectionWidget()
{
    delete ui;
}

void ConnectionWidget::setConnectMessage(QStringView msg) {
    ui->networkStatusLabel->setText(msg.toString());
    ui->statusComPortLabel->setText(msg.toString());
}

void ConnectionWidget::setConnected(bool isConnected) {
    qDebug() << "setConnected" << isConnected;
    if(isConnected) {
        ui->networkConnectButton->setText(tr("Disconnect"));
        ui->connectComPortButton->setText(tr("Disconnect"));
    } else {
        ui->networkConnectButton->setText(tr("Connect"));
        ui->connectComPortButton->setText(tr("Connect"));
    }
}

void ConnectionWidget::setBaudList(const QStringList& baudrateList){
    const QString current = ui->baudrateComboBox->currentText();
    ui->baudrateComboBox->clear();
    ui->baudrateComboBox->addItem("");
    ui->baudrateComboBox->addItems(baudrateList);
    if(baudrateList.contains(current))
        ui->baudrateComboBox->setCurrentText(current);
}

void ConnectionWidget::setPortList(const QStringList& portList){
    const QString current = ui->comPortComboBox->currentText();
    ui->comPortComboBox->clear();
    ui->comPortComboBox->addItem("");
    ui->comPortComboBox->addItems(portList);
    if(portList.contains(current))
        ui->comPortComboBox->setCurrentText(current);
}

void ConnectionWidget::setCurrentComPort(QStringView port){
    ui->comPortComboBox->setCurrentText(port.toString());
}

void ConnectionWidget::setCurrentIp(QStringView ip){
    ui->networkIpLineEdit->setText(ip.toString());
}

void ConnectionWidget::setCurrentTcpPort(int port){
    ui->networkPortLineEdit->setText(QString::number(port));
}

void ConnectionWidget::setProtocol(PortType type){

    if (type == PortType::TCP){
        ui->ConnectionTab->setCurrentWidget(ui->TcpTab);
    }
    else if(type == PortType::Com){
        ui->ConnectionTab->setCurrentWidget(ui->ComTab);
    }
    else
        return;
}

void ConnectionWidget::connectClicked(PortType type){
    QVariantMap networkMap;
    networkMap.insert("type",  static_cast<quint8>(type));
    if (type == PortType::TCP){
        networkMap.insert("host", ui->networkIpLineEdit->text());
        networkMap.insert("port", ui->networkPortLineEdit->text());
    }
    else if(type == PortType::Com){
        networkMap.insert("comport", ui->comPortComboBox->currentText());
        networkMap.insert("baudrate", ui->baudrateComboBox->currentText());
    }
    else
        return;

    emit changeConnectState(type, networkMap);
}

// private methods

void ConnectionWidget::paintEvent(QPaintEvent*) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
