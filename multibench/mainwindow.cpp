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
#include "widgets/plusminuswidget.h"
#include <QInputDialog>
#include <utility>
#include <widgets/rescanprogresswidget.h>
#include <widgets/quitdialog.h>

const int WidgetsInAppearence {2};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_portGroup(new QActionGroup(this))
    , m_baudrateGroup(new QActionGroup(this))
    , m_updater(new UpdateWidget(this))
    , m_About(new AboutDialog(this))
    , m_Quit(new QuitDialog(this))
    , m_connectionWidget(new ConnectionWidget(this))
{
    ui->setupUi(this);

    setFont(Const::ApplicationDefaultFontPath);

    this->move(AppSettings::getWindowPosition());

    auto netData = AppSettings::getNetworkData();

//    m_connectionWidget->setProtocol(PortType::TCP);
//    m_connectionWidget->setBaudList(Const::BaudRates);
//    switch(netData.type) {
//    case PortType::Com:
//        m_connectionWidget->setCurrentComPort(netData.host);
//        m_connectionWidget->setCurrentBautRate(netData.port);
//        break;
//    case PortType::TCP:
//        m_connectionWidget->setCurrentIp(netData.host);
//        m_connectionWidget->setCurrentTcpPort(netData.port);
//        break;
//    default:
//        break;
//    }

    connect(m_connectionWidget, &ConnectionWidget::refreshComPorts,
            this, &MainWindow::refreshComPortsSignal);
    connect(m_connectionWidget, &ConnectionWidget::changeConnectState,
            this, &MainWindow::changeConnectState);
    connect(m_connectionWidget, &ConnectionWidget::connectToCOM, this, &MainWindow::comTriggered);
    connect(m_connectionWidget, &ConnectionWidget::connectToTCP, this, &MainWindow::tcpTriggered);

    const QString AppTitle = QString("%1 v.%2").arg(Const::AppNameTitle, QCoreApplication::applicationVersion());
    setWindowTitle(AppTitle);
    setWindowIcon(QIcon(":/images/logo-minimal.png"));

    m_workFieldLayout = new QGridLayout(ui->scrollFieldWidget);
    m_workFieldLayout->setMargin(0);
    m_workFieldLayout->setSpacing(10);
    m_workFieldLayout->setContentsMargins(0,0,0,0);
    m_workFieldLayout->setSizeConstraint(QLayout::SetMinimumSize);
    m_workFieldLayout->setAlignment(Qt::AlignLeft);
    ui->scrollFieldWidget->setLayout(m_workFieldLayout);
    ui->scrollFieldWidget->setMaximumHeight(m_workFieldLayout->maximumSize().height());

    m_workFieldLayout->addWidget(m_connectionWidget);

    // setup menu's
//    ui->actionConnect->setCheckable(true);
//    ui->actionConnect->setChecked(false);
    connect(ui->actionRefresh_port, &QAction::triggered,this,&MainWindow::refreshComPortsSignal);
    connect(ui->actionConnect, &QAction::triggered,
            this, &MainWindow::comTriggered);

    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::closeAllWindows, Qt::QueuedConnection);
    auto temperatureGroup = new QActionGroup(this);
    //temperatureGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);
    temperatureGroup->setExclusive(true);
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
        AppSettings::setTemperatureUnit(id);
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

void MainWindow::comTriggered() {
    QVariantMap networkMap;
    networkMap.insert("type",  static_cast<quint8>(PortType::Com));
    qDebug() << "void MainWindow::connectTriggered()";
    if (!m_isConnected and m_portGroup->checkedAction() != nullptr and m_baudrateGroup->checkedAction() != nullptr){
        networkMap.insert("comport", m_portGroup->checkedAction()->text());
        networkMap.insert("baudrate", m_baudrateGroup->checkedAction()->text());
    }
    emit changeConnectState(PortType::Com, networkMap);
}

void MainWindow::tcpTriggered() {
    QVariantMap networkMap;
    networkMap.insert("type",  static_cast<quint8>(PortType::TCP));
    qDebug() << "void MainWindow::connectTriggered()";
    if (!m_isConnected){
        networkMap.insert("host", m_connectionWidget->getCurrentIp());
        networkMap.insert("port", m_connectionWidget->getCurrentTcpPort());
    }
    emit changeConnectState(PortType::TCP, networkMap);
}

void MainWindow::addDeviceWidget(DeviceWidget* widget) {
    if(!m_workWidgets.contains(widget)) {
        widget->setParent(this);
        m_workWidgets.append(widget);
        connect(widget, &DeviceWidget::nameEdited, this, &MainWindow::deviceNameChanged);
    }
}

void MainWindow::addCalibrationMenu(quint8 addr,quint16 id){
    auto action = new QAction(QString("ID:%1").arg(addr),this);
    connect(action,&QAction::triggered,this,[this,addr, id](){
        emit createCalibAndLimitsWidgets(addr, id);
    });
    ui->menuCalibration->addAction(action);
}

void MainWindow::rescanProgress(int current, int total, int success) {
    qDebug()<<"m_workWidgets.isEmpty()"<<m_workWidgets.isEmpty()<<current<<total<<success;
    if(current == 0 && total > 0) {
            m_progressWidget = new RescanProgressWidget(this);
            m_progressWidget->setProgress(current, total);
//            m_workFieldLayout->addItem(new QSpacerItem(10, 20), 0, 0);
            m_workFieldLayout->addWidget(m_progressWidget, 1, 0);
//            m_workFieldLayout->addItem(new QSpacerItem(10, 20), 2, 0);
//    } else if (success == 0 and total == current) {
//        QWidget *widgetRescan = new QWidget(this);
//        QVBoxLayout *rescanLayout = new QVBoxLayout(this);
//        QLabel *infoLabel = new QLabel("Devices don't found", this);
//        QPushButton *rescanButton = new QPushButton("Rescan", this);
//        rescanLayout->addWidget(infoLabel);
//        rescanLayout->addWidget(rescanButton);
//        widgetRescan->setLayout(rescanLayout);
//        m_workFieldLayout->addWidget(widgetRescan);
//        connect(rescanButton, &QPushButton::clicked, this, &MainWindow::comTriggered);
//        connect(rescanButton, &QPushButton::clicked, this, [this, widgetRescan](){
//            //m_workFieldLayout->removeWidget(widgetRescan);
//            widgetRescan->deleteLater();
//        });
    } else if (current == total) {
        if(m_progressWidget){
            if(!m_workWidgets.isEmpty()){
                m_progressWidget->setProgress(current, total);
                m_workFieldLayout->removeWidget(m_progressWidget);
                m_progressWidget->deleteLater();
            }
            else{
                m_progressWidget->setProgress(current, total);
                m_progressWidget->notFound();
            }
        }
        int maxWidth {-1};
        int widgetsCounter {0};
        int totalHeightInAppearence {0};
        for(auto widget : qAsConst(m_workWidgets)) {
            if(widget->width() > maxWidth) {
                maxWidth = widget->width();
            }
        }

        for(auto widget : qAsConst(m_workWidgets)) {
            widget->setConstraint(true);
            widget->setMinimumSize(maxWidth, widget->height());

            //widget->setConstraint(false);

            if(widgetsCounter < WidgetsInAppearence) {
                ++widgetsCounter;
                totalHeightInAppearence += widget->height();
                if (widgetsCounter>0)
                    totalHeightInAppearence += m_workFieldLayout->spacing();
            }
            m_workFieldLayout->addWidget(widget);
        }

        if(m_workWidgets.isEmpty()){
            maxWidth = ui->scrollArea->size().rwidth();
        }

        auto newSize = ui->scrollArea->size();
        int diffWidth = maxWidth-newSize.width();
        int diffHeight = totalHeightInAppearence - ui->scrollArea->height();

        newSize.rwidth() = maxWidth;

        if(diffHeight > 0)
            newSize.rheight() += diffHeight;
        ui->scrollArea->resize(newSize);

        auto winSize = size();
        winSize.rwidth() += diffWidth;
        winSize.rheight() += (diffHeight > 0) ? diffHeight : 0;
        resize(winSize);
    } else if(m_progressWidget) {
        m_progressWidget->setProgress(current, total);
    }

}

void MainWindow::setComPorts(const QStringList& portList) {
    m_connectionWidget->setPortList(portList);

    ui->menuPorts->clear();
    if (m_portGroup)
        m_portGroup->deleteLater();
    m_portGroup = new QActionGroup(this);
    for(const auto& port : portList) {
        auto action = new QAction(port);
        action->setCheckable(true);
        m_portGroup->addAction(action);
        ui->menuPorts->addAction(action);
        if (port == AppSettings::getNetworkData().port){
            action->setChecked(true);
            m_connectionWidget->setCurrentComPort(port);
        }
    }
    ui->menuPorts->addSeparator();
    connect(ui->menuPorts->addAction("Refresh"), &QAction::triggered,this,&MainWindow::refreshComPortsSignal);

    connect(m_portGroup, &QActionGroup::triggered, this, [this](QAction* action){
        setBothComPorts(action->text());
    });
    connect(m_connectionWidget, &ConnectionWidget::comPortIsChanged, this, &MainWindow::setBothComPorts);
}

void MainWindow::setBothComPorts(QString port){
    m_connectionWidget->setCurrentComPort(port);
    QList<QAction*> actions = m_portGroup->actions();
    for(auto item:actions){
        if(item->text() == port){
            item->setChecked(true);
        }
    }
}

void MainWindow::setBaudRates(const QStringList& baudrates) {
    m_connectionWidget->setBaudList(baudrates);

    ui->menuBaudrates->clear();
    if (m_baudrateGroup)
        m_baudrateGroup->deleteLater();
    m_baudrateGroup = new QActionGroup(this);
    for(const auto& baudrate : baudrates) {
        auto action = new QAction(baudrate);
        action->setCheckable(true);
        m_baudrateGroup->addAction(action);
        ui->menuBaudrates->addAction(action);
        if (baudrate.toInt() == AppSettings::getNetworkData().baudrate){
            action->setChecked(true);
            m_connectionWidget->setCurrentBautRate(baudrate);
        }
    }
    connect(m_baudrateGroup, &QActionGroup::triggered, this, [this](QAction* action){
        setBothBaudRates(action->text());
    });
    connect(m_connectionWidget, &ConnectionWidget::baudRateIsChanged, this, &MainWindow::setBothBaudRates);
}

void MainWindow::setBothBaudRates(QString baudrate){
    m_connectionWidget->setCurrentBautRate(baudrate);
    QList<QAction*> actions = m_baudrateGroup->actions();
    for(auto item:actions){
        if(item->text() == baudrate){
            item->setChecked(true);
        }
    }
}

void MainWindow::setConnectMessage(QString msg) {
    m_connectionWidget->setConnectMessage(msg);
}

void MainWindow::setConnected(bool isConnected) {
    m_connectionWidget->setConnected(isConnected);
    ui->menuPorts->setEnabled(!isConnected);
    ui->menuBaudrates->setEnabled(!isConnected);
    ui->actionRescan->setEnabled(isConnected);
    m_isConnected = isConnected;
    if (m_isConnected){
        ui->actionConnect->setText("Disconnect");
        m_connectionWidget->hide();
    }
    else{
        ui->actionConnect->setText("Connect");
        m_connectionWidget->show();
    }
    if(!isConnected) {
        for(auto widget : qAsConst(m_workWidgets)) {
            m_workFieldLayout->removeWidget(widget);
            widget->deleteLater();
        }
        if(m_progressWidget) {
            m_workFieldLayout->removeWidget(m_progressWidget);
            m_progressWidget->deleteLater();
        }
        m_workWidgets.clear();
        ui->menuCalibration->clear();
        adjustSize();
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
    m_Quit->show();
    event->ignore();
    connect(m_Quit, &QuitDialog::accepted, this, &QCoreApplication::quit);
}

void MainWindow::callAboutDialog(){
    m_About->show();
}

void MainWindow::setNetworkTimeout(){
    bool ok;
    int delay = QInputDialog::getInt(this,"Network Delay","Delay",
                                       AppSettings::getNetworkDelay(),
                                       Const::NetworkDelayMSecs::min,
                                       Const::NetworkDelayMSecs::max,
                                       1,&ok);
    if (ok)
        emit delayChanged(delay);
}

void MainWindow::triggeredRescanNetwork(){
    for(const auto item : std::as_const(m_workWidgets)){
        m_workFieldLayout->removeWidget(item);
        item->deleteLater();
    }
//    ui->actionKeepAddresses->setChecked(false);
//    AppSettings::setKeepAddresses(false);
    ui->menuCalibration->clear();
    m_workWidgets.clear();
    adjustSize();
    emit rescanNetwork();
}

void MainWindow::deviceNameChanged(QString name, int addr){
    QList<QAction*> actions = ui->menuCalibration->actions();
    for(auto item:actions){
        if(item->text().contains(QString("ID:%1").arg(addr))){
            if(name == "")
                item->setText(QString("ID:%1").arg(addr));
            else
                item->setText(QString(name + " ID:%1").arg(addr));
        }
    }
}

void MainWindow::slot_serialPortClosed(QString msg){
    setConnected(false);
    QuitDialog* portClosed = QuitDialog::createDialog("Error", "Failed, COM port is busy", QDialogButtonBox::Ok,12, this);
    connect(portClosed, &QuitDialog::accepted, portClosed, &QuitDialog::close);
}
