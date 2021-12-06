#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"
#include "widgets/connectionwidget.h"
#include "appsettings.h"
#include <QtWidgets>
#include <QSerialPortInfo>
#include <QDebug>
#include "widgets/updatewidget.h"
#include "widgets/aboutdialog.h"

//const QString MainWindow::SettingsPath {"window/"};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_portGroup(new QActionGroup(this))
    , m_baudrateGroup(new QActionGroup(this))
    , m_updater(new UpdateWidget(this))
    , m_About(new AboutDialog(this))
      //      m_cntrl(nullptr),
//      m_portList(nullptr),
//      m_baudList(nullptr)
{
    ui->setupUi(this);

    setFont(Const::ApplicationDefaultFontPath);

    this->move(AppSettings::getWindowPosition());

    auto netData = AppSettings::getNetworkData();
    #ifndef QT_DEBUG
    ui->connectionWidget->hide();
    #endif
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
            this, &MainWindow::refreshComPortsSignal);
    connect(ui->connectionWidget, &ConnectionWidget::connectButtonClicked,
            this, &MainWindow::connectToNetwork);

    const QString AppTitle = QString("%1 v.%2").arg(Const::AppNameTitle, QCoreApplication::applicationVersion());
    setWindowTitle(AppTitle);
    setWindowIcon(QIcon(":/images/logo-minimal.png"));

    m_workFieldLayout = new QGridLayout(ui->scrollFieldWidget);
    m_workFieldLayout->setMargin(0);
    m_workFieldLayout->setSpacing(10);
    m_workFieldLayout->setContentsMargins(0,0,0,0);
    m_workFieldLayout->setSizeConstraint(QLayout::SetMinimumSize);
    ui->scrollFieldWidget->setLayout(m_workFieldLayout);
    ui->scrollFieldWidget->setMaximumHeight(m_workFieldLayout->maximumSize().height());

    //    emit mainWindowReady();

    // setup menu's
    ui->actionConnect->setCheckable(true);
    ui->actionConnect->setChecked(false);
    //setBaudRates(Const::BaudRates);
    connect(ui->actionRefresh_port, &QAction::triggered,this,&MainWindow::refreshComPortsSignal);
    connect(ui->actionConnect, &QAction::triggered,
            this, &MainWindow::connectTriggered);
/*
    for(auto& port : m_baudrateGroup->actions()) {
        if (port->text().toUInt() == AppSettings::getComBaudrate()){
            port->setChecked(true);
        }
    }*/
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::closeAllWindows, Qt::QueuedConnection);
    auto temperatureGroup = new QActionGroup(this);
    temperatureGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);
    int unitIdx = static_cast<int>(AppSettings::getTemperatureUnit());
    for(const auto& unit : Const::TemperatureUnitNames) {
        auto action = new QAction(unit);
        action->setCheckable(true);
        action->setChecked(unit == Const::TemperatureUnitNames.at(unitIdx));
        ui->menuTmperature_units->addAction(action);
        temperatureGroup->addAction(action);
    }

    connect(temperatureGroup, &QActionGroup::triggered, this, [this](QAction* action){
        Const::TemperatureUnitId id = (action->text() == "Celsius") ?
                    Const::TemperatureUnitId::Celsius :
                    Const::TemperatureUnitId::Fahrenheit;

        emit tempratureUnitsChanged(id);

    });
    connect(m_updater,&UpdateWidget::downloadFinished,this,&MainWindow::updateDownloadingfinished);
    connect(ui->actionAbout,&QAction::triggered,this,&MainWindow::callAboutDialog);
    connect(ui->actionKeepAddresses, &QAction::triggered,this, &MainWindow::getKeepAddresses);
}

MainWindow::~MainWindow()
{
    //    if(m_cntrl != nullptr) delete m_cntrl;
//    if(m_portList != nullptr) delete m_portList;
//    if(m_baudList != nullptr) delete m_baudList;

    delete ui;
}

void MainWindow::connectTriggered(){
    QVariantHash networkMap;
    NetworkType type;
    type = NetworkType::SerialPort;
    networkMap.insert("type",  static_cast<quint8>(type));
    if (ui->actionConnect->isChecked() and m_portGroup->checkedAction() != nullptr and m_baudrateGroup->checkedAction() != nullptr){
        if (type == NetworkType::Tcp){
            networkMap.insert("host", "127.0.1.0");
            networkMap.insert("port", "9999");
        }
        else if(type == NetworkType::SerialPort){
            networkMap.insert("comport", m_portGroup->checkedAction()->iconText());
            networkMap.insert("baudrate", m_baudrateGroup->checkedAction()->iconText());
        }
        else
            return;
    }
    emit connectToNetwork(networkMap);
}

void MainWindow::addDeviceWidget(DeviceWidget* widget) {
    if(!m_workWidgets.contains(widget)) {
        m_workWidgets.append(widget);
        int count = m_workWidgets.size();
        qDebug() << "addDeviceWidget" << count;
        m_workFieldLayout->addWidget(widget);
        //        m_workFieldLayout->addItem(new QSpacerItem(2,2, QSizePolicy::Maximum, QSizePolicy::MinimumExpanding), count+1,0);
        //connect(widget, &DeviceWidget::sizeChanged, this, &MainWindow::adjust);
//        adjust(widget->sizeHint());
        //        widget->setMaximumHeight(widget->sizeHint().height());
        //        auto widgetSize = ui->workFieldWidget->size();
        qDebug() << "addDeviceWidget size=" << widget->size() << widget->sizeHint();
        //        ui->workFieldWidget->setMinimumSize(widgetSize);
        //        ui->scrollArea->setMinimumSize(widgetSize);
    }
}


void MainWindow::setComPorts(const QStringList& portList) {
    ui->menuPorts->clear();
    if (m_portGroup)
        m_portGroup->deleteLater();
    m_portGroup = new QActionGroup(this);
    for(const auto& port : portList) {
        auto action = new QAction(port);
        action->setCheckable(true);
        m_portGroup->addAction(action);
        ui->menuPorts->addAction(action);
        if (port == AppSettings::getComPort())
            action->setChecked(true);
        //m_portGroup->addAction(ui->menuPorts->addAction(port))->setCheckable(true);
    }
    ui->menuPorts->addSeparator();
    connect(ui->menuPorts->addAction("Refresh"), &QAction::triggered,this,&MainWindow::refreshComPortsSignal);


    ui->connectionWidget->setPortList(portList);
}

void MainWindow::setBaudRates(const QStringList& baudsList) {
    //ui->connectionWidget->setBaudList(baudsList);
    ui->menuBaudrates->clear();
    if (m_baudrateGroup)
        m_baudrateGroup->deleteLater();
    m_baudrateGroup = new QActionGroup(this);
    QStringList baudrates = baudsList;
    for(const auto& baudrate : baudrates) {
        auto action = new QAction(baudrate);
        action->setCheckable(true);
        m_baudrateGroup->addAction(action);
        ui->menuBaudrates->addAction(action);
        if (baudrate.toUInt() == AppSettings::getComBaudrate())
            action->setChecked(true);
        //m_baudrateGroup->addAction(ui->menuBaudrates->addAction(baudrate))->setCheckable(true);
    }
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
    ui->menuPorts->setEnabled(!isConnected);
    ui->menuBaudrates->setEnabled(!isConnected);
    if(!isConnected) {
        qDeleteAll(m_workWidgets);
        m_workWidgets.clear();
    }
}

void MainWindow::setStatusMessage(const QString& msg, int timeout) {
    ui->statusbar->showMessage(msg, timeout);
}

void MainWindow::getKeepAddresses(){
    AppSettings::setKeepAddresses(ui->actionKeepAddresses->isChecked());
}

// private methods
//void MainWindow::setConnections() {
// network connection button
//    connect(ui->networkConnectButton, &QPushButton::clicked, [=]{
//        emit this->networkConnectButtonSignal();


//    });
//}

//void MainWindow::adjust(const QSize& size) {
//    ui->workFieldWidget->adjustSize();
//    ui->workFieldWidget->setMinimumSize(ui->workFieldWidget->size());
//    if(!size.isEmpty()) {
//        ui->scrollArea->setMinimumWidth(size.width()+2*ui->scrollArea->frameWidth()+ui->scrollArea->verticalScrollBar()->sizeHint().width());
//    }
//    ui->workFieldWidget->updateGeometry();
//}

// protected methods

void MainWindow::closeEvent(QCloseEvent *event) {
    auto dialog = QMessageBox::question(this, "Do you want to quit?", "Do you really want to quit?");
    switch (dialog) {
    case QMessageBox::Yes:
        event->accept();
        break;
    case QMessageBox::No:
        event->ignore();
        break;
    default:
        break;
    }
}

void MainWindow::setStatusBarMessage(QString message){
    ui->statusbar->showMessage(message, Const::ConsoleMaxLinesToShow);
}

void MainWindow::updateDownloadingfinished(){
    ui->actionConnect->setCheckable(false);
    qDebug()<<"start update";
}

void MainWindow::callAboutDialog(){
    m_About->show();
}
