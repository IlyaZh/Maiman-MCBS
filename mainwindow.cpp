#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"
#include <QVariant>
#include <QScrollBar>
#include "widgets/connectionwidget.h"

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

    auto netData = m_settings.getNetworkData();
    ui->connectionWidget->setProtocol(NetworkType::Tcp);
    ui->connectionWidget->setBaudList(Const::BaudRates);
    switch(netData.type) {
    case NetworkType::SerialPort:
        ui->connectionWidget->setCurrentComPort(netData.host);
        break;
    case NetworkType::Tcp:
        ui->connectionWidget->setCurrentIp(netData.host);
        ui->connectionWidget->setCurrentTcpPort(netData.port);
        break;
    default:
        break;
    }

//    refreshMenuPortList();
//    refreshMenuPortBaudsList();

    connect(ui->connectionWidget, &ConnectionWidget::refreshComPorts,
            this, [this](){
        emit makeEvent("NetworkRefreshComPorts");
    });
    connect(ui->connectionWidget, &ConnectionWidget::connectButtonClicked,
            this, [this](QVariant value){
        emit makeEvent("NetworkConnectClicked", value);
    });

    const QString AppTitle = QString("%1 v.%2").arg(Const::AppNameTitle, QCoreApplication::applicationVersion());
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


void MainWindow::setComPorts(const QStringList& portList) {
    ui->connectionWidget->setPortList(portList);
}

void MainWindow::setBaudRates(const QStringList& baudsList) {
    ui->connectionWidget->setBaudList(baudsList);
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
    ui->connectionWidget->setConnectMessage(msg);
}

void MainWindow::setConnected(bool isConnected) {
    ui->connectionWidget->setConnected(isConnected);
}

// private methods
//void MainWindow::setConnections() {
    // network connection button
//    connect(ui->networkConnectButton, &QPushButton::clicked, [=]{
//        emit this->networkConnectButtonSignal();


//    });
//}

void MainWindow::adjust(const QSize& size) {
    ui->workFieldWidget->adjustSize();
    ui->workFieldWidget->setMinimumSize(ui->workFieldWidget->size());
    if(!size.isEmpty()) {
        ui->scrollArea->setMinimumWidth(size.width()+2*ui->scrollArea->frameWidth()+ui->scrollArea->verticalScrollBar()->sizeHint().width());
    }
}
