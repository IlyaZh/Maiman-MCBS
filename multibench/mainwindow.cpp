#include "mainwindow.h"

#include <widgets/quitdialog.h>
#include <widgets/rescanprogresswidget.h>

#include <QInputDialog>
#include <QSerialPortInfo>
#include <QtWidgets>
#include <utility>

#include "appsettings.h"
#include "constants.h"
#include "model/device/devicewidget.h"
#include "ui_mainwindow.h"
#include "widgets/aboutdialog.h"
#include "widgets/connectionwidget.h"
#include "widgets/updatewidget.h"

const int WidgetsInAppearence{2};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_portGroup(new QActionGroup(this)),
      m_baudrateGroup(new QActionGroup(this)),
      m_updater(new UpdateWidget(this)),
      m_About(new AboutDialog(this)),
      m_Quit(new QuitDialog(this)),
      m_connectionWidget(new ConnectionWidget(this)) {
  ui->setupUi(this);

  setFont(Const::kApplicationDefaultFontPath);

  this->move(AppSettings::getWindowPosition());

  auto netData = AppSettings::getNetworkData();

  connect(m_connectionWidget, &ConnectionWidget::refreshComPorts, this,
          &MainWindow::refreshComPortsSignal);
  connect(m_connectionWidget, &ConnectionWidget::changeConnectState, this,
          &MainWindow::changeConnectState);
  connect(m_connectionWidget, &ConnectionWidget::connectToCOM, this,
          &MainWindow::comTriggered);
  connect(m_connectionWidget, &ConnectionWidget::connectToTCP, this,
          &MainWindow::tcpTriggered);

  const QString AppTitle = QString("%1 v.%2").arg(
      Const::kAppNameTitle, QCoreApplication::applicationVersion());
  setWindowTitle(AppTitle);
  setWindowIcon(QIcon(":/images/logo-minimal.png"));

  m_workFieldLayout = new QGridLayout(ui->scrollFieldWidget);
  m_workFieldLayout->setMargin(0);
  m_workFieldLayout->setSpacing(10);
  m_workFieldLayout->setContentsMargins(0, 0, 0, 0);
  m_workFieldLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_workFieldLayout->setAlignment(Qt::AlignLeft);
  ui->scrollFieldWidget->setLayout(m_workFieldLayout);
  ui->scrollFieldWidget->setMaximumHeight(
      m_workFieldLayout->maximumSize().height());

  m_workFieldLayout->addWidget(m_connectionWidget);

  // setup menu's
  connect(ui->actionRefresh_port, &QAction::triggered, this,
          &MainWindow::refreshComPortsSignal);
  connect(ui->actionConnect, &QAction::triggered, this,
          &MainWindow::comTriggered);

  connect(ui->actionExit, &QAction::triggered, qApp,
          &QApplication::closeAllWindows, Qt::QueuedConnection);
  auto temperatureGroup = new QActionGroup(this);
  temperatureGroup->setExclusive(true);
  int unitIdx = static_cast<int>(AppSettings::getTemperatureUnit());
  for (const auto& unit : Const::kTemperatureUnitNames) {
    auto action = new QAction(unit);
    action->setCheckable(true);
    action->setChecked(unit == Const::kTemperatureUnitNames.at(unitIdx));
    ui->menuTmperature_units->addAction(action);
    temperatureGroup->addAction(action);
  }

  connect(temperatureGroup, &QActionGroup::triggered, this,
          [this](QAction* action) {
            Const::TemperatureUnitId id =
                (action->text() == "Celsius")
                    ? Const::TemperatureUnitId::kCelsius
                    : Const::TemperatureUnitId::kFahrenheit;
            AppSettings::setTemperatureUnit(id);
            emit tempratureUnitsChanged(id);
          });
  connect(ui->actionAbout, &QAction::triggered, this,
          &MainWindow::callAboutDialog);
  connect(ui->actionKeepAddresses, &QAction::triggered, this,
          &MainWindow::getKeepAddresses);
  connect(ui->actionRescan, &QAction::triggered, this,
          &MainWindow::triggeredRescanNetwork);
  connect(ui->actionDelay, &QAction::triggered, this,
          &MainWindow::setNetworkDelay);
  connect(ui->actionTimeout, &QAction::triggered, this,
          &MainWindow::setNetworkTimeout);
  connect(ui->actionMax_Address, &QAction::triggered, this,
          &MainWindow::setNetworkMaxAddress);
  ui->actionKeepAddresses->setChecked(AppSettings::getKeepAddresses());
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::comTriggered() {
  QVariantMap networkMap;
  networkMap.insert("type", static_cast<quint8>(Const::PortType::kCom));
  if (!m_isConnected and m_portGroup->checkedAction() != nullptr and
      m_baudrateGroup->checkedAction() != nullptr) {
    networkMap.insert("comport", m_portGroup->checkedAction()->text());
    networkMap.insert("baudrate", m_baudrateGroup->checkedAction()->text());
  }
  emit changeConnectState(Const::PortType::kCom, networkMap);
}

void MainWindow::tcpTriggered() {
  QVariantMap networkMap;
  networkMap.insert("type", static_cast<quint8>(Const::PortType::kTCP));
  if (!m_isConnected) {
    networkMap.insert("host", m_connectionWidget->getCurrentIp());
    networkMap.insert("port", m_connectionWidget->getCurrentTcpPort());
  }
  emit changeConnectState(Const::PortType::kTCP, networkMap);
}

void MainWindow::addDeviceWidget(DeviceWidget* widget) {
  if (!m_workWidgets.contains(widget)) {
    widget->setParent(this);
    m_workWidgets.append(widget);
    connect(widget, &DeviceWidget::nameEdited, this,
            &MainWindow::deviceNameChanged);
  }
}

void MainWindow::addCalibrationMenu(quint8 addr, quint16 id) {
  auto* action = new QAction(QString("ID:%1").arg(addr), this);
  connect(action, &QAction::triggered, this,
          [this, addr, id]() { emit createCalibAndLimitsWidgets(addr, id); });
  ui->menuCalibration->addAction(action);
}

void MainWindow::rescanProgress(int current, int total, int success) {
  if (current == 0 && total > 0) {
    m_progressWidget = new RescanProgressWidget(this);
    m_progressWidget->setProgress(current, total, success);
    m_workFieldLayout->addWidget(m_progressWidget, 1, 0);
  } else if (current == total) {
    if (m_progressWidget) {
      if (!m_workWidgets.isEmpty()) {
        m_progressWidget->setProgress(current, total, success);
        m_workFieldLayout->removeWidget(m_progressWidget);
        m_progressWidget->deleteLater();
      } else {
        m_progressWidget->setProgress(current, total, success);
        m_progressWidget->notFound();
      }
    }
    int maxWidth{-1};
    int widgetsCounter{0};
    int totalHeightInAppearence{0};
    for (auto* widget : qAsConst(m_workWidgets)) {
      if (widget->width() > maxWidth) {
        maxWidth = widget->width();
      }
    }

    for (auto* widget : qAsConst(m_workWidgets)) {
      widget->setConstraint(true);
      widget->setMinimumSize(maxWidth, widget->height());

      if (widgetsCounter < WidgetsInAppearence) {
        ++widgetsCounter;
        totalHeightInAppearence += widget->height();
        if (widgetsCounter > 0)
          totalHeightInAppearence += m_workFieldLayout->spacing();
      }
      m_workFieldLayout->addWidget(widget);
    }

    if (m_workWidgets.isEmpty()) {
      maxWidth = ui->scrollArea->size().rwidth();
    }

    auto newSize = ui->scrollArea->size();
    int diffWidth = maxWidth - newSize.width();
    int diffHeight = totalHeightInAppearence - ui->scrollArea->height();

    newSize.rwidth() = maxWidth;

    if (diffHeight > 0) newSize.rheight() += diffHeight;
    ui->scrollArea->resize(newSize);

    auto winSize = size();
    winSize.rwidth() += diffWidth;
    winSize.rheight() += (diffHeight > 0) ? diffHeight : 0;
    resize(winSize);
  } else if (m_progressWidget) {
    m_progressWidget->setProgress(current, total, success);
  }
}

void MainWindow::setComPorts(const QStringList& portList) {
  m_connectionWidget->setPortList(portList);

  ui->menuPorts->clear();
  if (m_portGroup) m_portGroup->deleteLater();
  m_portGroup = new QActionGroup(this);
  for (const auto& port : portList) {
    auto action = new QAction(port);
    action->setCheckable(true);
    m_portGroup->addAction(action);
    ui->menuPorts->addAction(action);
    if (port == AppSettings::getNetworkData().port) {
      action->setChecked(true);
      m_connectionWidget->setCurrentComPort(port);
    }
  }
  ui->menuPorts->addSeparator();
  connect(ui->menuPorts->addAction("Refresh"), &QAction::triggered, this,
          &MainWindow::refreshComPortsSignal);

  connect(m_portGroup, &QActionGroup::triggered, this,
          [this](QAction* action) { setBothComPorts(action->text()); });
  connect(m_connectionWidget, &ConnectionWidget::comPortIsChanged, this,
          &MainWindow::setBothComPorts);
}

void MainWindow::setBothComPorts(QString port) {
  m_connectionWidget->setCurrentComPort(port);
  QList<QAction*> actions = m_portGroup->actions();
  for (auto* item : actions) {
    if (item->text() == port) {
      item->setChecked(true);
    }
  }
}

void MainWindow::setBaudRates(const QStringList& baudrates) {
  m_connectionWidget->setBaudList(baudrates);

  ui->menuBaudrates->clear();
  if (m_baudrateGroup) m_baudrateGroup->deleteLater();
  m_baudrateGroup = new QActionGroup(this);
  for (const auto& baudrate : baudrates) {
    auto action = new QAction(baudrate);
    action->setCheckable(true);
    m_baudrateGroup->addAction(action);
    ui->menuBaudrates->addAction(action);
    if (baudrate.toInt() == AppSettings::getNetworkData().baudrate) {
      action->setChecked(true);
      m_connectionWidget->setCurrentBautRate(baudrate);
    }
  }
  connect(m_baudrateGroup, &QActionGroup::triggered, this,
          [this](QAction* action) { setBothBaudRates(action->text()); });
  connect(m_connectionWidget, &ConnectionWidget::baudRateIsChanged, this,
          &MainWindow::setBothBaudRates);
}

void MainWindow::setBothBaudRates(QString baudrate) {
  m_connectionWidget->setCurrentBautRate(baudrate);
  QList<QAction*> actions = m_baudrateGroup->actions();
  for (auto* item : actions) {
    if (item->text() == baudrate) {
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
  if (m_isConnected) {
    ui->actionConnect->setText("Disconnect");
    m_connectionWidget->hide();
  } else {
    ui->actionConnect->setText("Connect");
    m_connectionWidget->show();
  }
  if (!isConnected) {
    for (auto* widget : qAsConst(m_workWidgets)) {
      m_workFieldLayout->removeWidget(widget);
      widget->deleteLater();
    }
    if (m_progressWidget) {
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

void MainWindow::getKeepAddresses() {
  AppSettings::setKeepAddresses(ui->actionKeepAddresses->isChecked());
}

// protected methods

void MainWindow::closeEvent(QCloseEvent* event) {
  m_Quit->show();
  event->ignore();
  connect(m_Quit, &QuitDialog::accepted, this, &QCoreApplication::quit);
}

void MainWindow::callAboutDialog() { m_About->show(); }

void MainWindow::setNetworkDelay() {
  bool ok{false};
  int delay = QInputDialog::getInt(
      this, "Network Delay", "Delay", AppSettings::getNetworkDelay(),
      Const::kNetworkDelayMSecs::min, Const::kNetworkDelayMSecs::max, 1, &ok);
  if (ok) emit delayChanged(delay);
}

void MainWindow::setNetworkTimeout() {
  bool ok{false};
  int timeout = QInputDialog::getInt(this, "Network Timeout", "Timeout",
                                     AppSettings::getNetworkTimeout(),
                                     Const::kNetworkTimeoutMSecs::min,
                                     Const::kNetworkTimeoutMSecs::max, 1, &ok);
  if (ok) emit timeoutChanged(timeout);
}

void MainWindow::setNetworkMaxAddress() {
  bool ok{false};
  int address = QInputDialog::getInt(this, "Network Max Address", "Max Address",
                                     AppSettings::getNetworkMaxAddress(),
                                     Const::kNetworkMaxAddress::min,
                                     Const::kNetworkMaxAddress::max, 1, &ok);
  if (ok) AppSettings::setNetworkMaxAddress(address);
}

void MainWindow::triggeredRescanNetwork() {
  m_progressWidget->deleteLater();
  for (const auto item : std::as_const(m_workWidgets)) {
    m_workFieldLayout->removeWidget(item);
    item->deleteLater();
  }
  ui->menuCalibration->clear();
  m_workWidgets.clear();
  adjustSize();
  emit rescanNetwork();
}

void MainWindow::deviceNameChanged(QString name, int addr) {
  QList<QAction*> actions = ui->menuCalibration->actions();
  for (auto* item : actions) {
    if (item->text().contains(QString("ID:%1").arg(addr))) {
      if (name == "")
        item->setText(QString("ID:%1").arg(addr));
      else
        item->setText(QString(name + " ID:%1").arg(addr));
    }
  }
}

void MainWindow::slot_serialPortClosed(const QString& /*msg*/) {
  setConnected(false);
  QuitDialog* portClosed = QuitDialog::createDialog(
      "Error", "Failed, COM port is busy", QDialogButtonBox::Ok, 12, this);
  connect(portClosed, &QuitDialog::accepted, portClosed, &QuitDialog::close);
}

void MainWindow::emptyNetwork() {
  QuitDialog* portClosed = QuitDialog::createDialog(
      "Error", "All devices disconnected", QDialogButtonBox::Ok, 12, this);
  connect(portClosed, &QuitDialog::accepted, portClosed, &QuitDialog::close);
}

void MainWindow::NewEvent(const model::Event& event) {
  std::get<model::events::network::StateUpdated>(event.data_).reg;
}
