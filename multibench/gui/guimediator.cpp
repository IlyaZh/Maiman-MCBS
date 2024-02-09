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
  connect(&window, &MainWindow::createGroupManagerWidget, this,
          &GuiMediator::createGroupManagerWidget);
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
    //    connect(device, &Device::linkChanged, widget, &DeviceWidget::setLink);
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

void GuiMediator::createGroupManagerWidget() {
  QPointer<GroupManager> manager(m_factory.createGroupManagerWidget(
      m_deviceWidgetsTable, m_groupWidgetsTable));
  manager->setModal(false);
  manager->show();
  connect(manager, &GroupManager::createGroupWidget, this,
          &GuiMediator::createGroupWidgetFor);
  connect(manager, &GroupManager::deleteGroupWidget, this,
          &GuiMediator::deleteGroupWidgetFor);
  connect(this, &GuiMediator::repaintGroupsAndDevices, manager,
          &GroupManager::finishGroupAction);
  connect(manager, &GroupManager::removeMemberGroup, this,
          [this](int groupAddr, quint8 devAddr) {
            emit modifMemberGroup(true, groupAddr, devAddr);
          });
  connect(manager, &GroupManager::addMemberGroup, this,
          [this](int groupAddr, quint8 devAddr) {
            emit modifMemberGroup(false, groupAddr, devAddr);
          });
}

void GuiMediator::createGroupWidgetFor(const QSet<quint8>& addresses,
                                       int groupAddr) {
  QPointer<GroupWidget> group(m_factory.createGroupWidget(groupAddr));
  auto addrs = addresses.values();
  std::sort(addrs.begin(), addrs.end());
  for (const auto addr : addrs) {
    auto widget = m_deviceWidgetsTable.value(addr);
    group->addGroupMember(widget);
    m_window.removeDeviceWidget(widget);
  }
  m_window.addGroupWidget(group);
  m_groupWidgetsTable.insert(group->getGroupAddress(), group);
  connect(group, &GroupWidget::groupEvent, this,
          &GuiMediator::Signal_PublishEvent);
  emit repaintGroupsAndDevices();
}

void GuiMediator::deleteGroupWidgetFor(int address) {
  m_window.removeGroupWidget(m_groupWidgetsTable.value(address));
  for (auto addr : m_groupWidgetsTable.value(address)->getAddresses()) {
    auto widget = m_deviceWidgetsTable.value(addr);
    m_window.addDeviceWidget(widget);
  }
  m_groupWidgetsTable.remove(address);
  m_window.restoreDeviceWidgets();

  emit repaintGroupsAndDevices();
}

void GuiMediator::modifMemberGroup(bool isRemove, int groupAddr,
                                   quint8 devAddr) {
  if (isRemove) {
    m_groupWidgetsTable.value(groupAddr)->removeGroupMember(
        m_deviceWidgetsTable.value(devAddr));
    m_window.addDeviceWidget(m_deviceWidgetsTable.value(devAddr));
    m_window.restoreDeviceWidgets();
  } else {
    m_groupWidgetsTable.value(groupAddr)->addGroupMember(
        m_deviceWidgetsTable.value(devAddr));
    m_window.removeDeviceWidget(m_deviceWidgetsTable.value(devAddr));
  }

  emit repaintGroupsAndDevices();
}

void GuiMediator::NewEvent(const model::Event& event) {
  if (event.type_ == model::EventType::kDeviceStateUpdated) {
    if (std::holds_alternative<model::events::network::Answer>(event.data_)) {
      auto addr = std::get<model::events::network::Answer>(event.data_).addr_;
      m_deviceWidgetsTable.value(addr)->updateValue(event);
      if (m_calibrationDialog.contains(addr)) {
        m_calibrationDialog.value(addr)->updateValue(event);
      }
      for (const auto& group : m_groupWidgetsTable) {
        auto data = std::get<model::events::network::Answer>(event.data_);
        if (group->getAddresses().contains(data.addr_)) {
          auto status = m_factory.deviceErrorStatus(
              m_deviceWidgetsTable[data.addr_]->getId(), data.reg_,
              data.value_);
          group->setDevicesStatus(data.addr_, status);
        }
      }
    } else if (std::holds_alternative<model::events::network::DeviceLinkStatus>(
                   event.data_)) {
      auto data =
          std::get<model::events::network::DeviceLinkStatus>(event.data_);
      m_deviceWidgetsTable.value(data.addr_)->setLink(data.status_);
    }
  }
}

void GuiMediator::dataCapture(quint8 addr, quint16 code, quint16 value) {
  model::Event event(
      model::EventType::kWriteDevice,
      model::events::network::SingleWriteRequest(addr, code, value));
  emit Signal_PublishEvent(event);
}
