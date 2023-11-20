#include "guimediator.h"

#include <QSerialPortInfo>

#include "device/device.h"
#include "mainwindow.h"
#include "model/device/devicewidget.h"
#include "model/guifactory.h"
#include "widgets/calibrationdialog.h"

GuiMediator::GuiMediator(MainWindow& window, GuiFactory& factory,
                         NetworkModel& networkModel, QObject* parent)
    : QObject(parent),
      m_window(window),
      m_factory(factory),
      m_network(networkModel) {
  factory.start();
  connect(&networkModel, &NetworkModel::signal_createWidgetFor, this,
          &GuiMediator::createWidgetFor);
  connect(&window, &MainWindow::createCalibAndLimitsWidgets, this,
          &GuiMediator::createCalibAndLimitsWidgets);
}

void GuiMediator::createWidgetFor(Device* device) {
  // TODO: пронеси Device мимо этого класса в наследуемые
  //  m_converters.createConverters(device->addr(),
  //                                m_network.getCommands(device->addr()));
  QPointer<DeviceWidget> widget(m_factory.createDeviceWidget(
      device->id(), device->commands(), device->converters()));
  if (widget) {
    m_deviceWidgetsTable.insert(device->addr(), widget);
    widget->setAddress(static_cast<int>(device->addr()));
    connect(device, &Device::linkChanged, widget, &DeviceWidget::setLink);
    connect(widget, &DeviceWidget::acceptDataFromWidget, this,
            [this, device](quint16 code, quint16 value) {
              dataCapture(device->addr(), code, value);
            });
    m_window.addDeviceWidget(widget);
    if (m_factory.hasCalibration(device->id()) or
        m_factory.hasLimits(device->id()))
      m_window.addCalibrationMenu(device->addr(), device->id());
  } else {
    qWarning() << "Can't find device widget with id=" << device->id();
  }
}

void GuiMediator::createCalibAndLimitsWidgets(quint8 addr, quint16 id) {
  if (!m_calibrationDialog.value(addr)) {
    CalibrationDialog* dialog =
        m_factory.createCalibrationDialog(id, m_network.getConverters(addr));
    dialog->setModal(false);
    dialog->show();
    m_calibrationDialog.insert(addr, QSharedPointer<CalibrationDialog>(dialog));
    connect(dialog, &CalibrationDialog::finished, this,
            [this, addr]() { m_calibrationDialog.remove(addr); });
    connect(dialog, &CalibrationDialog::acceptDataFromWidget, this,
            [this, addr](quint16 code, quint16 value) {
              dataCapture(addr, code, value);
            });
  }
}

void GuiMediator::NewEvent(const model::Event& event) {
  if (event.type_ == model::EventType::kDeviceStateUpdated) {
    if (std::holds_alternative<model::events::network::Answer>(event.data_)) {
      auto addr = std::get<model::events::network::Answer>(event.data_).addr_;
      m_deviceWidgetsTable.value(addr)->updateValue(event);
      if (m_calibrationDialog.contains(addr)) {
        m_calibrationDialog.value(addr)->updateValue(event);
      }
    }
  }
}

void GuiMediator::dataCapture(quint8 addr, quint16 code, quint16 value) {
  model::Event event(
      model::EventType::kWriteDevice,
      model::events::network::SingleWriteRequest(addr, code, value));
  emit Signal_PublishEvent(event);
}
