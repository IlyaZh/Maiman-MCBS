#include "modelguimediator.h"
#include "appsettings.h"
#include <QSerialPortInfo>
#include "model/device/devicewidget.h"
#include "device/device.h"
#include "mainwindow.h"
#include "model/guifactory.h"
#include "widgets/calibratedialog.h"

ModelGuiMediator::ModelGuiMediator(MainWindow& window, GuiFactory& factory,NetworkModel& networkModel,QObject *parent) :
    QObject(parent),
    m_window(window),
    m_factory(factory),
    m_network(networkModel)
{
    factory.start();
    connect(&networkModel, &NetworkModel::createWidgetFor,this,&ModelGuiMediator::createWidgetFor);
    connect(&networkModel, &NetworkModel::setBaudrateToWindow,this,&ModelGuiMediator::setBaudrateToWindow);

    connect(&window, &MainWindow::connectToNetwork, this, &ModelGuiMediator::connectToNetwork);
    connect(&window, &MainWindow::refreshComPortsSignal, this, &ModelGuiMediator::refreshComPorts);
    connect(&window, &MainWindow::tempratureUnitsChanged, &m_network, &NetworkModel::temperatureUnitsChanged);
    refreshComPorts();
    connect(&window,&MainWindow::rescanNetwork,this,&ModelGuiMediator::rescan);
    connect(&window,&MainWindow::createCalibAndLimitsWidgets,this,&ModelGuiMediator::createCalibAndLimitsWidgets);
}

void ModelGuiMediator::createWidgetFor(Device* device) {
    QPointer<DeviceWidget> widget(m_factory.createDeviceWidget(device->id(), device->commands()));
    if(widget) {
        widget->setAddress(static_cast<int>(device->addr()));
        connect(device, &Device::linkChanged, widget, &DeviceWidget::setLink);
        m_window.addDeviceWidget(widget);
        m_window.addCalibrationDialog(device->addr(),device->id());
        //m_window.addCalibrationDialog(device->addr(),m_factory.createDeviceCalibrationWidget(device->id(), device->commands()),m_factory.createDeviceLimitsWidget(device->id(), device->commands()));
    } else {
        qWarning() << "Can't find device widget with id=" << device->id();
    }
}

void ModelGuiMediator::createCalibAndLimitsWidgets(quint8 addr, quint16 id){
    CalibrateDialog* dialog = m_factory.createCalibrationAndLimitsDialog(id,m_network.getCommands(addr));
    //dialog->setParent(&m_window);
    dialog->show();
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

void ModelGuiMediator::connectToNetwork(QVariant value) {
    AppSettings::setNetworkData(value);

    QVariantMap portSettings = value.toMap();
    if(m_network.isStart()) {
        m_network.stop();
        m_device->close();
        m_window.setConnected(m_device->isOpen());
        //            device->deleteLater();
    } else {
        NetworkType type = static_cast<NetworkType>(portSettings["type"].toInt());
        if(type == NetworkType::Tcp) {
            m_device = new DataSource;
            m_device->setSettings(type, portSettings["host"], portSettings["port"]);
            m_device->open();
            m_window.setConnected(m_device->isOpen());
            m_network.start(*m_device);
        } else if(type == NetworkType::SerialPort) {
            m_device = new DataSource;
            AppSettings::setComPort(portSettings["comport"].toString());
            AppSettings::setComBaudrate(portSettings["baudrate"].toInt());
            m_device->setSettings(type, portSettings["comport"], portSettings["baudrate"]);
            m_device->open();
            m_window.setConnected(m_device->isOpen());
            m_network.start(*m_device);
        }
    }
}

void ModelGuiMediator::rescan(){
    m_network.clearNetwork();
    m_network.rescanNetwork();
}
