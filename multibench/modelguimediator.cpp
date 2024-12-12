#include "modelguimediator.h"

#include <QSerialPortInfo>

#include "appsettings.h"
#include "device/device.h"
#include "mainwindow.h"
#include "model/device/devicewidget.h"
#include "model/guifactory.h"
#include "network/IDataSource.h"
#include "network/datasourcefactory.h"
#include "widgets/calibrationdialog.h"

NetworkMediator::NetworkMediator(MainWindow& window, GuiFactory& factory,
                                 NetworkModel& networkModel, QObject* parent)
    : QObject(parent),
      m_window(window),
      m_factory(factory),
      m_network(networkModel) {
  connect(&networkModel, &NetworkModel::signal_setBaudrateToWindow, this,
          &NetworkMediator::setBaudrateToWindow);
  connect(&networkModel, &NetworkModel::signal_connected, &window,
          &MainWindow::setConnected);

  connect(&window, &MainWindow::changeConnectState, this,
          &NetworkMediator::changeConnectState);
  connect(&window, &MainWindow::refreshComPortsSignal, this,
          &NetworkMediator::refreshComPorts);
  connect(&window, &MainWindow::tempratureUnitsChanged, &m_network,
          &NetworkModel::temperatureUnitsChanged);
  refreshComPorts();
  connect(&window, &MainWindow::rescanNetwork, this, &NetworkMediator::rescan);

  connect(&window, &MainWindow::delayChanged, &networkModel,
          &NetworkModel::setDelay);
  connect(&window, &MainWindow::timeoutChanged, &networkModel,
          &NetworkModel::setTimeout);
  connect(&networkModel, &NetworkModel::signal_rescanProgress, &window,
          &MainWindow::rescanProgress);

  connect(&networkModel, &NetworkModel::signal_errorOccured, &m_window,
          &MainWindow::slot_serialPortClosed);
  connect(&networkModel, &NetworkModel::signal_emptyNetwork, &m_window,
          &MainWindow::emptyNetwork);
}

void NetworkMediator::setBaudrateToWindow(QStringList baud) {
  m_window.setBaudRates(baud);
}

void NetworkMediator::refreshComPorts() {
  QStringList ports;
  const auto availablePorts = QSerialPortInfo::availablePorts();
  for (const auto& port : availablePorts) {
    ports << port.portName();
  }
  m_window.setComPorts(ports);
}

void NetworkMediator::changeConnectState(Const::PortType type,
                                         QVariantMap portSettings) {
  if (m_network.isStart()) {
    m_network.stop();
  } else {
    if (!portSettings.isEmpty()) {
      AppSettings::setNetworkData(portSettings);

      auto dataSource =
          QScopedPointer<IDataSource>(DataSourceFactory::createSource(type));
      if (dataSource) {
        dataSource->init(portSettings);
        m_network.start(dataSource);
      }
    }
  }
}

void NetworkMediator::rescan() {
  m_network.clearNetwork();
  m_network.rescanNetwork();
}
