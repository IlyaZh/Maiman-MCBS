#include "groupholder.h"

#include "group/groupcommandfactory.h"

enum { foldedHeight = 110, expandedHeight = 200 };

GroupHolder::GroupHolder(int groupAddr, QWidget *parent)
    : QWidget(parent),
      m_stacked(new QStackedLayout(this)),
      m_selfAddr(groupAddr) {
  m_name = QString("Group %1").arg(m_selfAddr);
  this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

  m_expandedWidget = new GroupWidget(this);
  m_foldedWidget = new GroupWidgetFolded(this);

  m_expandedWidget->setName(m_name);
  m_foldedWidget->setName(m_name);

  m_stacked->addWidget(m_expandedWidget);
  m_stacked->addWidget(m_foldedWidget);

  m_stacked->setCurrentWidget(m_expandedWidget);
  m_stacked->setMargin(0);
  m_stacked->setSpacing(0);
  this->setLayout(m_stacked);

  connect(m_expandedWidget, &GroupWidget::startAll, this,
          &GroupHolder::startDevices);
  connect(m_expandedWidget, &GroupWidget::stopAll, this,
          &GroupHolder::stopDevices);
  connect(m_expandedWidget, &GroupWidget::hideWidget, this,
          &GroupHolder::hideGroup);

  connect(m_foldedWidget, &GroupWidgetFolded::startAll, this,
          &GroupHolder::startDevices);
  connect(m_foldedWidget, &GroupWidgetFolded::stopAll, this,
          &GroupHolder::stopDevices);
  connect(m_foldedWidget, &GroupWidgetFolded::showWidget, this,
          &GroupHolder::showGroup);
}

void GroupHolder::addGroupMember(QPointer<DeviceHolder> member) {
  m_expandedWidget->addGroupMember(member);
  m_addresses.insert(static_cast<quint8>(member->getAddress()));
  m_linked.insert(static_cast<quint8>(member->getAddress()), true);
}

void GroupHolder::removeGroupMember(QPointer<DeviceHolder> member) {
  m_expandedWidget->removeGroupMember(member);
  m_addresses.remove(static_cast<quint8>(member->getAddress()));
  m_status.remove(static_cast<quint8>(member->getAddress()));
  m_linked.remove(static_cast<quint8>(member->getAddress()));
}

void GroupHolder::startDevices() {
  auto command = model::events::network::CommandType::kStartDevices;
  m_allStarted = true;
  emit groupEvent(
      GroupCommandFactory::createGroupCommand(m_addresses, command));
  //  ui->startButton->setStyleSheet(Widget::buttonLaunched());
  //  ui->stopButton->setStyleSheet(Widget::buttonInMiddle());
}

void GroupHolder::stopDevices() {
  auto command = model::events::network::CommandType::kStopDevices;
  m_allStarted = false;
  emit groupEvent(
      GroupCommandFactory::createGroupCommand(m_addresses, command));
  //  ui->startButton->setStyleSheet(Widget::buttonInMiddle());
  //  ui->stopButton->setStyleSheet(Widget::buttonStopped());
}

void GroupHolder::setDevicesStatus(quint8 addr,
                                   QSharedPointer<DeviceStatusGroup> desc) {
  if (!m_addresses.contains(addr)) return;
  if (desc.isNull()) return;
  if (!m_status.contains(addr)) {
    m_status.insert(addr, desc);
  }
  if (desc.data()->isStarted.has_value()) {
    m_status[addr]->isStarted = desc.data()->isStarted;
  }
  if (desc.data()->isError.has_value() and desc.data()->isError) {
    m_status[addr]->errors->clear();
    if (desc.data()->errors.has_value())
      m_status[addr]->errors->append(desc.data()->errors.value());
    m_status[addr]->errors->removeDuplicates();
  } else {
    m_status[addr]->errors->clear();
  }
  if (desc.data()->isInterlock.has_value() and desc.data()->isInterlock) {
    m_status[addr]->interlocks->clear();
    if (desc.data()->interlocks.has_value())
      m_status[addr]->interlocks->append(desc.data()->interlocks.value());
    m_status[addr]->interlocks->removeDuplicates();
  } else {
    m_status[addr]->interlocks->clear();
  }
  m_expandedWidget->addDevicesData(m_status);
  m_foldedWidget->addDevicesData(m_status);
}

const QString GroupHolder::getName() { return m_name; }

void GroupHolder::setName(QString name) {
  m_name = name;
  m_expandedWidget->setName(name);
  m_foldedWidget->setName(name);
}

void GroupHolder::hideGroup(bool flag) {
  m_hideDevices = flag;
  m_stacked->setCurrentWidget(m_foldedWidget);
  setMinimumSize(QSize(m_foldedWidget->minimumSize().width(), foldedHeight));
  resize(QSize(m_foldedWidget->minimumSize().width(), foldedHeight));
  QWidget *current = m_stacked->currentWidget();
  if (current) {
    // Ширину НЕ трогаем
    setMinimumHeight(foldedHeight);
    setMaximumHeight(foldedHeight);
    // Ширина — по политике, например:
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    updateGeometry();  // позволяет родителю пересчитать layout
  }
}

void GroupHolder::showGroup(bool flag) {
  m_hideDevices = !flag;
  m_stacked->setCurrentWidget(m_expandedWidget);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  setMinimumHeight(0);
  setMaximumHeight(QWIDGETSIZE_MAX);
  updateGeometry();
}

const QSet<quint8> GroupHolder::getAddresses() { return m_addresses; }

int GroupHolder::getGroupAddress() { return m_selfAddr; }

void GroupHolder::linkStatusChanged(int addr, bool status) {
  m_linked[static_cast<quint8>(addr)] = status;
  bool groupLink = true;
  for (auto &linked : m_linked) {
    groupLink = groupLink & linked;
  }
  m_expandedWidget->linkStatusChanged(groupLink);
  m_foldedWidget->linkStatusChanged(groupLink);
  emit linkChanged(addr, status);
}

void GroupHolder::updateValue(const model::Event &event) {
  if (event.type_ == model::EventType::kSystemCommand) {
    if (std::holds_alternative<model::events::network::ChangeSystemStyle>(
            event.data_)) {
      m_expandedWidget->updateStyle();
      m_foldedWidget->updateStyle();
    }
  }
}

void GroupHolder::updateStyle() {}
