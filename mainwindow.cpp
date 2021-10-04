#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globals.h"
#include <QVariant>
#include <QScrollBar>

#include <QDebug>

const QString MainWindow::SettingsPath {"window/"};

MainWindow::MainWindow(AppSettings& settings, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
//      m_cntrl(nullptr),
      m_portList(nullptr),
      m_baudList(nullptr),
      m_settings(settings)
{
    ui->setupUi(this);

    this->move(m_settings.getWindowPosition());

    NetworkData_s netData;
    netData = m_settings.getNetworkData();
    if(netData.type != NetworkType::NONE) {
        ui->ipLineEdit->setText(netData.host);
        ui->portSpinBox->setValue(netData.port);
    }

    // ip reg-ex for connection fields
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    // You may want to use QRegularExpression for new code with Qt 5 (not mandatory).
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->ipLineEdit->setValidator(ipValidator);

    setConnections();

//    refreshMenuPortList();
//    refreshMenuPortBaudsList();

    static const QString AppTitle = QString("%1 v.%2").arg(Constants::AppNameTitle, QCoreApplication::applicationVersion());
    setWindowTitle(AppTitle);
    setWindowIcon(QIcon(":/images/logo-minimal.png"));

    m_workFieldLayout = new QGridLayout(ui->workFieldWidget);
    m_workFieldLayout->setMargin(0);
    m_workFieldLayout->setSpacing(0);
    m_workFieldLayout->setContentsMargins(0,0,0,0);
    ui->workFieldWidget->setLayout(m_workFieldLayout);

//    emit mainWindowReady();
}

MainWindow::~MainWindow()
{
//    if(m_cntrl != nullptr) delete m_cntrl;
    if(m_portList != nullptr) delete m_portList;
    if(m_baudList != nullptr) delete m_baudList;

    delete ui;
}

//void MainWindow::addSettingsCntrl(AppSettings* settings) {
//    m_settings = settings;
//}

/*void MainWindow::addController(MainWindowControllerInterface* cntrl) {
    clearController();
    m_cntrl = cntrl;
}*/

/*void MainWindow::clearController() {
    if(m_cntrl != nullptr) {
        delete m_cntrl;
        m_cntrl = nullptr;
    }
}*/

void MainWindow::setConnectMessage(QString msg) {
    ui->networkStateLabel->setText(msg);
}

void MainWindow::setConnected(bool isConnected) {
    qDebug() << "MainWindow::setConnected" << isConnected;
    if(isConnected) {
        ui->networkConnectButton->setText(tr("Disconnect"));
    } else {
        ui->networkConnectButton->setText(tr("Connect"));
    }
}


void MainWindow::setPortList(QVector<QString> *portList) {
    if(m_portList != nullptr) {
        delete m_portList;
    }
    m_portList = portList;
}

void MainWindow::setBaudList(QVector<int> *baudList) {
    if(m_baudList != nullptr) {
        delete m_baudList;
    }
    m_baudList = baudList;
}

// private methods
void MainWindow::setConnections() {
    // network connection button
//    connect(ui->networkConnectButton, &QPushButton::clicked, [=]{
//        emit this->networkConnectButtonSignal();


//    });
}

void MainWindow::on_networkConnectButton_clicked()
{
//    emit networkConnectClicked(NetworkType::TCP, ui->ipLineEdit->text(), ui->portSpinBox->value());

//    NetworkData_s netData;
//    netData.type = NetworkType::TCP;
//    netData.host = ui->ipLineEdit->text();
//    netData.port = ui->portSpinBox->value();
    QVariantHash networkMap;
    networkMap.insert("type", static_cast<quint8>(NetworkType::Tcp));
    networkMap.insert("host", ui->ipLineEdit->text());
    networkMap.insert("port", ui->portSpinBox->value());
    emit makeEvent("NetworkConnectClicked", networkMap);
    /*if(m_cntrl != nullptr) {
        m_cntrl->networkConnectClicked(TCP_PROTOCOL, ui->ipLineEdit->text(), ui->portSpinBox->value());

        if(m_settings != nullptr) {
            NetworkData_s netData;
            netData.type = TCP_PROTOCOL;
            netData.host = ui->ipLineEdit->text();
            netData.port = ui->portSpinBox->value();
            m_settings->setNetworkData(netData);
        }
    }*/
}

void MainWindow::adjust(const QSize& size) {
    ui->workFieldWidget->adjustSize();
    ui->workFieldWidget->setMinimumSize(ui->workFieldWidget->size());
    if(!size.isEmpty()) {
        ui->scrollArea->setMinimumWidth(size.width()+2*ui->scrollArea->frameWidth()+ui->scrollArea->verticalScrollBar()->sizeHint().width());
    }
}

void MainWindow::addDeviceWidget(DeviceWidget* widget) {
    if(!m_workWidgets.contains(widget)) {
        m_workWidgets.append(widget);
        int count = m_workFieldLayout->layout()->count();
        m_workFieldLayout->addWidget(widget, count, 0);
        m_workFieldLayout->addItem(new QSpacerItem(2,2, QSizePolicy::Maximum, QSizePolicy::MinimumExpanding), count+1,0);
        connect(widget, &DeviceWidget::sizeChanged, this, &MainWindow::adjust);
        adjust(widget->size());
//        auto widgetSize = ui->workFieldWidget->size();
//        ui->workFieldWidget->setMinimumSize(widgetSize);
//        ui->scrollArea->setMinimumSize(widgetSize);
    }
}
