#include "guimediator.h"

#include <QSerialPortInfo>

#include "device/device.h"
#include "mainwindow.h"
#include "model/device/devicewidget.h"
#include "model/guifactory.h"
#include "widgets/calibrationdialog.h"

CommandConverter::CommandConverter(const CommandSettings& conf, quint16 value)
    : m_config(conf), m_rawValue(value) {
  double d = 0;
  if (m_config.m_isSigned) {
    d = static_cast<double>(static_cast<int16_t>(m_rawValue));
    m_value =
        qRound(d / m_config.m_divider * qPow(10, m_config.m_tolerance) - 0.5) /
        qPow(10, m_config.m_tolerance);
  } else {
    d = static_cast<double>(static_cast<quint16>(m_rawValue));
    m_value =
        qRound(d / m_config.m_divider * qPow(10, m_config.m_tolerance) - 0.5) /
        qPow(10, m_config.m_tolerance);
  }
}

QString CommandConverter::unit() const {
  if (m_config.m_isTemperature) {
    switch (m_tempId) {
      default:
      case Const::TemperatureUnitId::kCelsius:
        return m_config.m_unit + "C";
        break;
      case Const::TemperatureUnitId::kFahrenheit:
        return m_config.m_unit + "F";
        break;
    }
  } else {
    return m_config.m_unit;
  }
}

void CommandConverter::changeTemperatureUnit(Const::TemperatureUnitId id) {
  if (m_config.m_isTemperature) {
    m_tempId = id;
    switch (id) {
      default:
      case Const::TemperatureUnitId::kCelsius:
        m_value = convertFarToCel();
        break;
      case Const::TemperatureUnitId::kFahrenheit:
        m_value = convertCelToFar();
        break;
    }
  }
}

double CommandConverter::convertCelToFar() {
  return m_value * 9.0 / 5.0 + 32.0;
}

double CommandConverter::convertFarToCel() {
  return (m_value - 32.0) * 5.0 / 9.0;
}

double CommandConverter::divider() const { return m_config.m_divider; }

int CommandConverter::tolerance() const {
  return static_cast<int>(m_config.m_tolerance);
}

bool CommandConverter::isSigned() const { return m_config.m_isSigned; }

bool CommandConverter::isTemperature() const {
  return m_config.m_isTemperature;
}

uint CommandConverter::interval() const { return m_config.m_interval; }

double CommandConverter::valueDouble() const { return m_value; }

uint CommandConverter::valueInt() const { return static_cast<int>(m_value); }

QString CommandConverter::valueStr() const {
  return QString::number(m_value, 'f', static_cast<int>(m_config.m_tolerance));
}

quint16 CommandConverter::code() { return m_config.m_code; }

//-----------------------------------

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
  QPointer<DeviceWidget> widget(
      m_factory.createDeviceWidget(device->id(), device->commands()));
  if (widget) {
    m_deviceWidgetsTable.insert(device->addr(), widget);
    widget->setAddress(static_cast<int>(device->addr()));
    connect(device, &Device::linkChanged, widget, &DeviceWidget::setLink);
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
        m_factory.createCalibrationDialog(id, m_network.getCommands(addr));
    dialog->setModal(false);
    dialog->show();
    m_calibrationDialog.insert(addr, id);
    connect(dialog, &CalibrationDialog::finished, this,
            [this, addr]() { m_calibrationDialog.remove(addr); });
  }
}

void GuiMediator::NewEvent(const model::Event& event) {
  if (event.type_ == model::EventType::kDeviceStateUpdated) {
    auto addr = std::get<model::events::network::Answer>(event.data_).address;
    auto code = std::get<model::events::network::Answer>(event.data_).reg;
    auto value = std::get<model::events::network::Answer>(event.data_).value;
    auto command = QSharedPointer<CommandConverter>(new CommandConverter(
        m_network.getCommands(addr)[code]->commands(), value));
    command.data()->changeTemperatureUnit(
        Const::TemperatureUnitId::kCelsius);  // изменение температуры
    m_deviceWidgetsTable[addr]->updateValue(command);
  }
}
