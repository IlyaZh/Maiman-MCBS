#include "modelguimediator.h"
#include "appsettings.h"
#include <QSerialPortInfo>
#include "model/device/devicewidget.h"
#include "device/device.h"
#include "mainwindow.h"
#include "model/guifactory.h"
#include "widgets/calibratedialog.h"
#include "SerialThreadWorker.h"

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

    connect(&window, &MainWindow::connectToNetwork, this, &ModelGuiMediator::connectToNetwork);
    connect(&window, &MainWindow::refreshComPortsSignal, this, &ModelGuiMediator::refreshComPorts);
    connect(&window, &MainWindow::tempratureUnitsChanged, &m_network, &NetworkModel::temperatureUnitsChanged);
    refreshComPorts();
    connect(&window,&MainWindow::rescanNetwork,this,&ModelGuiMediator::rescan);
}

void ModelGuiMediator::createWidgetFor(Device* device) {
    QPointer<DeviceWidget> widget(m_factory.createDeviceWidget(device->id(), device->commands()));
    if(widget) {
        widget->setAddress(static_cast<int>(device->addr()));
        connect(device, &Device::linkChanged, widget, &DeviceWidget::setLink);
        m_window.addDeviceWidget(widget);

        m_window.addCalibrationDialog(device->addr(),m_factory.createDeviceCalibrationWidget(device->id(), device->commands()),m_factory.createDeviceLimitsWidget(device->id(), device->commands()));
    } else {
        qWarning() << "Can't find device widget with id=" << device->id();
    }
}

void ModelGuiMediator::createCalibAndLimitsWidgets(quint8 addr) {

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

void ModelGuiMediator:: connectToNetwork(PortType type, QVariant value) {
    QVariantMap portSettings = value.toMap();
    if(m_network.isStart()) {
        m_network.stop();
//        m_device->close();
    } else {
        AppSettings::setNetworkData(value);
        auto serialWorker = new SerialThreadWorker;
        if(type == PortType::TCP) {
            serialWorker->configure(type, portSettings["host"], portSettings["port"]);
        } else if(type == PortType::Com) {
            serialWorker->configure(type, portSettings["comport"], portSettings["baudrate"]);;
        } else {
            serialWorker->deleteLater();
            return;
        }

        m_network.start(serialWorker);
    }
}

void ModelGuiMediator::rescan(){
    m_network.clearNetwork();
    m_network.rescanNetwork();
}
