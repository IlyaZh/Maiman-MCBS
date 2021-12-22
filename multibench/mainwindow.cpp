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
#include "widgets/calibrationdialog.h"
#include "widgets/calibrationandlimitswidget.h"
#include <QInputDialog>
#include <utility>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_portGroup(new QActionGroup(this))
    , m_baudrateGroup(new QActionGroup(this))
    , m_updater(new UpdateWidget(this))
    , m_About(new AboutDialog(this))
{
    ui->setupUi(this);

    setFont(Const::ApplicationDefaultFontPath);

    this->move(AppSettings::getWindowPosition());

    auto netData = AppSettings::getNetworkData();
    #ifndef QT_DEBUG
    ui->connectionWidget->hide();
    #endif
    ui->connectionWidget->setProtocol(PortType::TCP);
    ui->connectionWidget->setBaudList(Const::BaudRates);
    switch(netData.type) {
    case PortType::Com:
        ui->connectionWidget->setCurrentComPort(netData.host);
        break;
    case PortType::TCP:
        ui->connectionWidget->setCurrentIp(netData.host);
        ui->connectionWidget->setCurrentTcpPort(netData.port);
        break;
    default:
        break;
    }

    connect(ui->connectionWidget, &ConnectionWidget::refreshComPorts,
            this, &MainWindow::refreshComPortsSignal);
    connect(ui->connectionWidget, &ConnectionWidget::changeConnectState,
            this, &MainWindow::changeConnectState);

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

    // setup menu's
    ui->actionConnect->setCheckable(true);
    ui->actionConnect->setChecked(false);
    connect(ui->actionRefresh_port, &QAction::triggered,this,&MainWindow::refreshComPortsSignal);
    connect(ui->actionConnect, &QAction::triggered,
            this, &MainWindow::connectTriggered);

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
    connect(ui->actionAbout,&QAction::triggered,this,&MainWindow::callAboutDialog);
    connect(ui->actionKeepAddresses, &QAction::triggered,this, &MainWindow::getKeepAddresses);
    connect(ui->actionRescan,&QAction::triggered,this,&MainWindow::triggeredRescanNetwork);
    connect(ui->actionTimeout,&QAction::triggered,this,&MainWindow::setNetworkTimeout);
    ui->actionKeepAddresses->setChecked(AppSettings::getKeepAddresses());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectTriggered(){
    QVariantMap networkMap;
    auto type = PortType::Com;
    networkMap.insert("type",  static_cast<quint8>(type));
    if (ui->actionConnect->isChecked() and m_portGroup->checkedAction() != nullptr and m_baudrateGroup->checkedAction() != nullptr){
        if (type == PortType::TCP){
            networkMap.insert("host", "127.0.1.0");
            networkMap.insert("port", "9999");
        }
        else if(type == PortType::Com){
            networkMap.insert("comport", m_portGroup->checkedAction()->text());
            networkMap.insert("baudrate", m_baudrateGroup->checkedAction()->text());
        }
        else
            return;
    }
    emit changeConnectState(type, networkMap);
}

void MainWindow::addDeviceWidget(DeviceWidget* widget) {
    if(!m_workWidgets.contains(widget)) {
        widget->setParent(this);
        m_workWidgets.append(widget);
        m_workFieldLayout->addWidget(widget);
    }
}

void MainWindow::addCalibrationMenu(quint8 addr,quint16 id){
    auto action = new QAction(QString("ID:%1").arg(addr),this);
    connect(action,&QAction::triggered,this,[this,addr, id](){
        emit createCalibAndLimitsWidgets(addr, id);
    });
    ui->menuCalibration->addAction(action);
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
    }
    ui->menuPorts->addSeparator();
    connect(ui->menuPorts->addAction("Refresh"), &QAction::triggered,this,&MainWindow::refreshComPortsSignal);


    ui->connectionWidget->setPortList(portList);
}

void MainWindow::setBaudRates(const QStringList& baudsList) {
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
        ui->menuCalibration->clear();
    }
}

void MainWindow::setStatusMessage(const QString& msg, int timeout) {
    ui->statusbar->showMessage(msg, timeout);
}

void MainWindow::getKeepAddresses(){
    AppSettings::setKeepAddresses(ui->actionKeepAddresses->isChecked());
}

// protected methods

void MainWindow::closeEvent(QCloseEvent *event) {
    auto result = QMessageBox::question(this, "Do you want to quit?", "Do you really want to quit?");
    event->ignore();
    if(result == QMessageBox::Yes)
        qApp->quit();
}

void MainWindow::setStatusBarMessage(QString message){
    ui->statusbar->showMessage(message, Const::ConsoleMaxLinesToShow);
}

void MainWindow::callAboutDialog(){
    m_About->show();
}

void MainWindow::setNetworkTimeout(){
    bool ok;
    quint16 timeout = QInputDialog::getInt(this,"Network Timeout","Timeout",AppSettings::getNetworkTimeout(),0,1000,1,&ok);
    if (ok)
        emit timeoutChanged(timeout);
}

void MainWindow::triggeredRescanNetwork(){
    for(const auto item : std::as_const(m_workWidgets)){
        m_workFieldLayout->removeWidget(item);
    }
    ui->menuCalibration->clear();
    m_workWidgets.clear();
    emit rescanNetwork();
}
