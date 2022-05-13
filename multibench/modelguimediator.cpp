#include "modelguimediator.h"
#include "appsettings.h"
#include <QSerialPortInfo>
#include "model/device/devicewidget.h"
#include "device/device.h"
#include "mainwindow.h"
#include "model/guifactory.h"
#include "widgets/calibrationdialog.h"
#include "network/IDataSource.h"
#include "network/datasourcefactory.h"

ModelGuiMediator::ModelGuiMediator(MainWindow& window, GuiFactory& factory,NetworkModel& networkModel,QObject *parent) :
    QObject(parent),
    m_window(window),
    m_factory(factory),
    m_network(networkModel)
{
    factory.start();
    connect(&networkModel, &NetworkModel::signal_createWidgetFor,this,&ModelGuiMediator::createWidgetFor);
    connect(&networkModel, &NetworkModel::signal_setBaudrateToWindow,this,&ModelGuiMediator::setBaudrateToWindow);
    connect(&networkModel, &NetworkModel::signal_connected, &window, &MainWindow::setConnected);

    connect(&window, &MainWindow::changeConnectState, this, &ModelGuiMediator::changeConnectState);
    connect(&window, &MainWindow::refreshComPortsSignal, this, &ModelGuiMediator::refreshComPorts);
    connect(&window, &MainWindow::tempratureUnitsChanged, &m_network, &NetworkModel::temperatureUnitsChanged);
    refreshComPorts();
    connect(&window, &MainWindow::rescanNetwork, this, &ModelGuiMediator::rescan);
    connect(&window, &MainWindow::createCalibAndLimitsWidgets, this, &ModelGuiMediator::createCalibAndLimitsWidgets);

    connect(&window, &MainWindow::delayChanged, &networkModel, &NetworkModel::setDelay);
    connect(&networkModel, &NetworkModel::signal_rescanProgress, &window, &MainWindow::rescanProgress);

    connect(&networkModel, &NetworkModel::signal_errorOccured, this, [this](QString msg){
        qDebug()<<"error"<<msg;
        if(msg.contains("Can't сonnect."))
            m_window.setConnected(false);
    });
}

void ModelGuiMediator::createWidgetFor(Device* device) {
    // TODO: пронеси Device мимо этого класса в наследуемые
    QPointer<DeviceWidget> widget(m_factory.createDeviceWidget(device->id(), device->commands()));
    if(widget) {
        widget->setAddress(static_cast<int>(device->addr()));
        connect(device, &Device::linkChanged, widget, &DeviceWidget::setLink);
        m_window.addDeviceWidget(widget);
        if(m_factory.hasCalibration(device->id()) or m_factory.hasLimits(device->id()))
            m_window.addCalibrationMenu(device->addr(),device->id());
    } else {
        qWarning() << "Can't find device widget with id=" << device->id();
    }
}

void ModelGuiMediator::createCalibAndLimitsWidgets(quint8 addr, quint16 id){
    if(!m_calibrationDialog.value(addr)){
        CalibrationDialog* dialog = m_factory.createCalibrationDialog(id,m_network.getCommands(addr));
        dialog->setModal(false);
        dialog->show();
        m_calibrationDialog.insert(addr, id);
        connect(dialog, &CalibrationDialog::finished, this, [this, addr](){
            m_calibrationDialog.remove(addr);
        });

    }
}

void ModelGuiMediator::setBaudrateToWindow(QStringList baud) {
    m_window.setBaudRates(baud);
}

void ModelGuiMediator::refreshComPorts() {
    QStringList ports;
    const auto availablePorts = QSerialPortInfo::availablePorts();
    for(const auto& port : availablePorts) {
        ports << port.portName();
    }
    m_window.setComPorts(ports);

}

void ModelGuiMediator::changeConnectState(PortType type, QVariantMap portSettings) {
    if(m_network.isStart()) {
        m_network.stop();
    } else {
        if(!portSettings.isEmpty()){
            AppSettings::setNetworkData(portSettings);

            auto dataSource = QScopedPointer<IDataSource>(DataSourceFactory::createSource(type));
            if(dataSource) {
                dataSource->init(portSettings);
                m_network.start(dataSource);
            }
        }
    }
}

void ModelGuiMediator::rescan(){
    m_network.clearNetwork();
    m_network.rescanNetwork();
}

