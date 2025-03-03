#include "guimediator.h"

#include <QSerialPortInfo>

#include "device/device.h"
#include "mainwindow.h"
#include "model/device/devicewidget.h"
#include "model/guifactory.h"
#include "network/IDataSource.h"
#include "network/datasourcefactory.h"
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
  connect(&window, &MainWindow::clearWidgets, this, &GuiMediator::clear);
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
    dialog->setModal(true);
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
  manager->show();
  connect(manager, &GroupManager::sendAllGroups, this,
          &GuiMediator::recreateGroups);
}

void GuiMediator::deleteGroupWidgetFor(int address) {
  m_window.removeGroupWidget(m_groupWidgetsTable.value(address));
  for (auto addr : m_groupWidgetsTable.value(address)->getAddresses()) {
    auto widget = m_deviceWidgetsTable.value(addr);
    m_window.addDeviceWidget(widget);
  }
  m_groupWidgetsTable.remove(address);
}

void GuiMediator::recreateGroups(
    const QMap<int, QSharedPointer<groupCheckBoxes>>& groups) {
  auto oldGroupAddr = m_groupWidgetsTable.keys();
  if (!oldGroupAddr.isEmpty()) {
    for (auto addr : oldGroupAddr) {
      deleteGroupWidgetFor(addr);
    }
  }
  if (!groups.isEmpty()) {
    for (auto& group : qAsConst(groups)) {
      QPointer<GroupWidget> groupWidget(
          m_factory.createGroupWidget(group->g_addr));
      groupWidget->setName(group->g_checkBox->text());
      auto addrs = group->g_subBoxes.keys();
      std::sort(addrs.begin(), addrs.end());
      for (auto addr : addrs) {
        auto widget = m_deviceWidgetsTable.value(addr);
        groupWidget->addGroupMember(widget);
        m_window.removeDeviceWidget(widget);
      }
      m_window.addGroupWidget(groupWidget);
      m_groupWidgetsTable.insert(groupWidget->getGroupAddress(), groupWidget);
      connect(groupWidget, &GroupWidget::groupEvent, this,
              &GuiMediator::Signal_PublishEvent);

      //      if (groups.size() > 1) {
      //        m_window.repaintGroupsToTabs(groupWidget);
      //      }
    }
  }
  //  if (groups.size() <= 1) {
  //    m_window.repaintGroupToScroll();
  //  }
  m_window.restoreDeviceWidgets();
}

void GuiMediator::NewEvent(const model::Event& event) {
  if (event.type_ == model::EventType::kDeviceStateUpdated) {
    if (std::holds_alternative<model::events::network::Answer>(event.data_)) {
      auto addr = std::get<model::events::network::Answer>(event.data_).addr_;
      m_deviceWidgetsTable.value(addr)->updateValue(event);
      if (m_calibrationDialog.contains(addr)) {
        m_calibrationDialog.value(addr)->updateValue(event);
      }
      for (auto& group : m_groupWidgetsTable) {
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
      for (auto& group : m_groupWidgetsTable) {
        if (group->getAddresses().contains(data.addr_)) {
          group->linkStatusChanged(data.addr_, data.status_);
        }
      }
    }
  } else if (event.type_ == model::EventType::kSystemCommand) {
    if (std::holds_alternative<model::events::network::ChangeSystemStyle>(
            event.data_)) {
      for (auto& device : m_deviceWidgetsTable) {
        device->updateValue(event);
      }
      for (auto& group : m_groupWidgetsTable) {
        group->updateValue(event);
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

void GuiMediator::clear() {
  m_deviceWidgetsTable.clear();
  m_groupWidgetsTable.clear();
  m_calibrationDialog.clear();
}
