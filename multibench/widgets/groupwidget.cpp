#include "groupwidget.h"

#include "group/groupcommandfactory.h"
#include "ui_groupwidget.h"

using namespace StyleStorage::Group;

const int WidgetsInAppearence{2};

GroupWidget::GroupWidget(int groupAddr, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::GroupWidget),
      m_widgetLayout(new QGridLayout()),
      m_selfAddr(groupAddr) {
  ui->setupUi(this);
  this->setObjectName("GroupWidget");
  m_warning = new WarningWidget();
  ui->launchTable->addWidget(m_warning, Qt::AlignLeft);
  m_widgetLayout = new QGridLayout(this);
  m_widgetLayout->setMargin(0);
  m_widgetLayout->setSpacing(10);
  m_widgetLayout->setContentsMargins(0, 0, 0, 0);
  m_widgetLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_widgetLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  ui->devicesTable->setLayout(m_widgetLayout);
  ui->gridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_hideButton = new GroupHideButton(this);
  m_statusButton = new GroupHideButton(this);
  m_statusButton->setCheckable(false);
  m_hideButton->setText(" " + tr("Hide Devices"));
  m_statusButton->setText(" " + tr("Minimize All"));
  ui->statusButtontable->insertWidget(1, m_hideButton);
  ui->statusButtontable->insertWidget(2, m_statusButton);
  ui->statusButtontable->setAlignment(Qt::AlignLeft);
  m_name = QString("Group %1").arg(m_selfAddr);
  ui->labelName->setText(m_name);
  //  m_name = new InLineEdit(m_selfAddr, false);
  //  ui->nameTable->insertWidget(1, m_name, Qt::AlignmentFlag::AlignLeft);
  ui->nameTable->setAlignment(Qt::AlignmentFlag::AlignLeft);

  connect(ui->startButton, &QPushButton::clicked, this,
          &GroupWidget::startDevices);
  connect(ui->stopButton, &QPushButton::clicked, this,
          &GroupWidget::stopDevices);
  connect(m_hideButton, &QPushButton::clicked, this, &GroupWidget::hideDevices);
  connect(m_statusButton, &QPushButton::clicked, this,
          &GroupWidget::showStatus);
  //  connect(m_name, &InLineEdit::nameEdited, this, &GroupWidget::nameEdited);
  GroupWidget::updateStyle();
  findHiddenDevices();
}

GroupWidget::~GroupWidget() { delete ui; }

void GroupWidget::addGroupMember(QPointer<DeviceHolder> member) {
  //  if (member == this) return;
  //  m_widgetLayout->addWidget(member);
  m_groupWidgets.append(member);
  qDebug() << "add new member" << member->size() << member->sizeHint();
  resizeWidget();
  qDebug() << "add new member" << member->size() << member->sizeHint();
  emit closeGroupStatusDialog();
  m_addresses.insert(static_cast<quint8>(member->getAddress()));
  m_linked.insert(static_cast<quint8>(member->getAddress()), true);
  connect(member, &DeviceHolder::hideStatus, this,
          &GroupWidget::isHiddenWidget);
}

void GroupWidget::removeGroupMember(QPointer<DeviceHolder> member) {
  //  if (member == this) return;
  emit closeGroupStatusDialog();
  m_widgetLayout->removeWidget(member);
  m_groupWidgets.removeOne(member);
  m_addresses.remove(static_cast<quint8>(member->getAddress()));
  m_status.remove(static_cast<quint8>(member->getAddress()));
  m_linked.remove(static_cast<quint8>(member->getAddress()));
  disconnect(member, &DeviceHolder::hideStatus, this,
             &GroupWidget::isHiddenWidget);
}

const QSet<quint8> GroupWidget::getAddresses() { return m_addresses; }

void GroupWidget::startDevices() {
  auto command = model::events::network::CommandType::kStartDevices;
  m_allStarted = true;
  emit groupEvent(
      GroupCommandFactory::createGroupCommand(m_addresses, command));
  ui->startButton->setStyleSheet(Widget::buttonLaunched());
  ui->stopButton->setStyleSheet(Widget::buttonInMiddle());
}

void GroupWidget::stopDevices() {
  auto command = model::events::network::CommandType::kStopDevices;
  m_allStarted = false;
  emit groupEvent(
      GroupCommandFactory::createGroupCommand(m_addresses, command));
  ui->startButton->setStyleSheet(Widget::buttonInMiddle());
  ui->stopButton->setStyleSheet(Widget::buttonStopped());
}

void GroupWidget::resizeWidget() {
  int maxWidth{-1}, maxHeight{0};
  int widgetsCounter{0};
  int totalHeightInAppearence{0};
  for (auto &widget : qAsConst(m_groupWidgets)) {
    widget->adjustSize();
    if (widget->width() > maxWidth) {
      maxWidth = widget->width();
    }
    m_widgetLayout->removeWidget(widget);
  }
  std::sort(m_groupWidgets.begin(), m_groupWidgets.end(),
            [](DeviceHolder *a, DeviceHolder *b) {
              return a->getAddress() < b->getAddress();
            });
  for (auto widget : qAsConst(m_groupWidgets)) {
    widget->setConstraint(true);
    widget->resize(maxWidth, widget->height());
    maxHeight += widget->height();
    if (widgetsCounter < WidgetsInAppearence) {
      ++widgetsCounter;
      totalHeightInAppearence += widget->minimumHeight();
      if (widgetsCounter > 0)
        totalHeightInAppearence += m_widgetLayout->spacing();
    }
    //    qDebug() << "Holder valid:" << widget->size() << widget->sizeHint();
    //    widget->setStyleSheet("background: green;");
    m_widgetLayout->addWidget(widget);
  }
  maxHeight += m_widgetLayout->spacing() * (m_widgetLayout->count() - 1);
  //  ui->devicesTable->setMinimumHeight(100);
  //  ui->devicesTable->setStyleSheet("background: red;");
  //  this->setStyleSheet("background: blue;");
  auto newSize = ui->devicesTable->size();
  //  int diffHeight = totalHeightInAppearence - ui->devicesTable->height();
  newSize.rwidth() = maxWidth + 15;

  newSize.rheight() = maxHeight;
  ui->devicesTable->resize(newSize);
  ui->devicesTable->adjustSize();
  ui->devicesTable->update();
  this->adjustSize();
  this->setMinimumSize(this->size());
  qDebug() << "group size" << ui->devicesTable->size() << this->size();
}

void GroupWidget::hideDevices(bool flag) {
  m_hideDevices = flag;

  if (!m_hideDevices) {
    m_hideButton->setText(" " + tr("Hide Devices"));
    ui->devicesTable->setVisible(true);
    //    this->layout()->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
    this->adjustSize();
    this->setMinimumWidth(this->size().rwidth());
  } else {
    m_hideButton->setText(" " + tr("Show Devices"));
    ui->devicesTable->setVisible(false);
    //    this->layout()->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
    this->adjustSize();
    this->setMinimumWidth(this->size().rwidth());
  }

  resizeWidget();
}

void GroupWidget::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void GroupWidget::setDevicesStatus(quint8 addr,
                                   QSharedPointer<DeviceStatusGroup> desc) {
  if (!m_addresses.contains(addr)) return;
  if (desc.isNull()) return;
  if (!m_status.contains(addr)) {
    m_status.insert(addr, desc);
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
  //  emit statusChanged(m_status);
  m_warning->addDevicesData(m_status);
}

const QString GroupWidget::getName() { return m_name; }

void GroupWidget::setName(QString name) {
  m_name = name;
  ui->labelName->setText(m_name);
  //  if (name.contains(QString("Group %1").arg(m_selfAddr)))
  //    m_name->setAddress(m_selfAddr);
  //  else
  //    m_name->setText(name);
}

int GroupWidget::getGroupAddress() { return m_selfAddr; }
void GroupWidget::linkStatusChanged(int addr, bool status) {
  m_linked[static_cast<quint8>(addr)] = status;
  bool groupLink = true;
  for (auto &linked : m_linked) {
    groupLink = groupLink & linked;
  }
  if (!groupLink) {
    ui->linkLabel->setStyleSheet(StyleStorage::Device::linkDisconnected());
  } else {
    ui->linkLabel->setStyleSheet(StyleStorage::Device::linkConnected());
  }
  emit linkChanged(addr, status);
}

void GroupWidget::showStatus() {
  if (findHiddenDevices()) {
    for (auto device : qAsConst(m_groupWidgets)) {
      device->showWidgetButtonClicked();
    }
  } else {
    for (auto device : qAsConst(m_groupWidgets)) {
      device->hideControlsButtonClicked();
    }
  }
  resizeWidget();
}

void GroupWidget::updateValue(const model::Event &event) {
  if (event.type_ == model::EventType::kSystemCommand) {
    if (std::holds_alternative<model::events::network::ChangeSystemStyle>(
            event.data_)) {
      updateStyle();
    }
  }
}

void GroupWidget::updateStyle() {
  this->setStyleSheet(Widget::groupWidget());
  this->update();
  QList<QWidget *> widgets = this->findChildren<QWidget *>();
  for (QWidget *widget : widgets) {
    GuiWidgetInterface *interfaceWidget =
        dynamic_cast<GuiWidgetInterface *>(widget);
    if (interfaceWidget) {
      interfaceWidget->updateStyle();
    }
  }
  if (m_allStarted) {
    ui->startButton->setStyleSheet(Widget::buttonLaunched());
    ui->stopButton->setStyleSheet(Widget::buttonInMiddle());
  } else {
    ui->startButton->setStyleSheet(Widget::buttonInMiddle());
    ui->stopButton->setStyleSheet(Widget::buttonStopped());
  }
}

bool GroupWidget::findHiddenDevices() {
  bool isHidden = false;
  for (auto device : qAsConst(m_groupWidgets)) {
    if (device->isHide()) {
      isHidden = true;
    }
  }
  if (isHidden) {
    m_statusButton->setText(" " + tr("Maximize All"));
  } else {
    m_statusButton->setText(" " + tr("Minimize All"));
  }
  return isHidden;
}

void GroupWidget::isHiddenWidget(bool state) {
  if (state) {
    findHiddenDevices();
  } else {
    findHiddenDevices();
  }
  resizeWidget();
  ui->devicesTable->adjustSize();
  qDebug() << "adjust" << ui->devicesTable->size()
           << ui->devicesTable->sizeHint();
  emit sizeChanged();
}
