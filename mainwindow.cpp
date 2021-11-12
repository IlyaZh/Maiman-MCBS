#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"
#include "widgets/connectionwidget.h"
#include "appsettings.h"
#include <QtWidgets>

#include <QDebug>

//const QString MainWindow::SettingsPath {"window/"};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
      //      m_cntrl(nullptr),
//      m_portList(nullptr),
//      m_baudList(nullptr)
{
    ui->setupUi(this);

    setFont(Const::ApplicationDefaultFontPath);

    this->move(AppSettings::getWindowPosition());

    auto netData = AppSettings::getNetworkData();
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

    m_workFieldLayout = new QVBoxLayout(ui->scrollFieldWidget);
    m_workFieldLayout->setMargin(0);
    m_workFieldLayout->setSpacing(10);
    m_workFieldLayout->setContentsMargins(0,0,0,0);
    m_workFieldLayout->setSizeConstraint(QLayout::SetMinimumSize);
    ui->scrollFieldWidget->setLayout(m_workFieldLayout);
    ui->scrollFieldWidget->setMaximumHeight(m_workFieldLayout->maximumSize().height());
    emit refreshComPortsSignal();

    //    emit mainWindowReady();

    // setup menu's
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::closeAllWindows, Qt::QueuedConnection);
    auto temperatureGroup = new QActionGroup(this);
    temperatureGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);
    for(const auto& unit : Const::TemperatureUnits) {
        auto action = new QAction(unit);
        ui->menuTmperature_units->addAction(action);
        temperatureGroup->addAction(action);
    }

    connect(temperatureGroup, &QActionGroup::triggered, this, [this](QAction* action){
        Const::TemperatureUnitId id = (action->text() == "Celsius") ?
                    Const::TemperatureUnitId::Celsius :
                    Const::TemperatureUnitId::Fahrenheit;

        emit tempratureUnitsChanged(id);

    });
}

MainWindow::~MainWindow()
{
    //    if(m_cntrl != nullptr) delete m_cntrl;
//    if(m_portList != nullptr) delete m_portList;
//    if(m_baudList != nullptr) delete m_baudList;

    delete ui;
}

void MainWindow::addDeviceWidget(DeviceWidget* widget) {
    if(!m_workWidgets.contains(widget)) {
        m_workWidgets.append(widget);
        int count = m_workWidgets.size();
        qDebug() << "addDeviceWidget" << count;
        m_workFieldLayout->addWidget(widget);
        //        m_workFieldLayout->addItem(new QSpacerItem(2,2, QSizePolicy::Maximum, QSizePolicy::MinimumExpanding), count+1,0);
        connect(widget, &DeviceWidget::sizeChanged, this, &MainWindow::adjust);
        adjust(widget->sizeHint());
        //        widget->setMaximumHeight(widget->sizeHint().height());
        //        auto widgetSize = ui->workFieldWidget->size();
        qDebug() << "addDeviceWidget size=" << widget->size() << widget->sizeHint();
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

    if(!isConnected) {
        qDeleteAll(m_workWidgets);
        m_workWidgets.clear();
    }
}

void MainWindow::setStatusMessage(const QString& msg, int timeout) {
    ui->statusbar->showMessage(msg, timeout);
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
    ui->workFieldWidget->updateGeometry();
}

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
