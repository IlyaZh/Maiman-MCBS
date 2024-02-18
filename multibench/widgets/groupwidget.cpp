#include "groupwidget.h"

#include "group/groupcommandfactory.h"
#include "ui_groupwidget.h"

static const QString buttonOn =
    "QPushButton \
                                   { \
                                       border: 2px solid rgb(26,26,26); \
                                       border-radius: 6px; \
                                       color: rgb(0,0,0); \
                                       background: rgb(0,102,51); \
                                   }";

static const QString buttonOff =
    "QPushButton \
                                   { \
                                       border: 2px solid rgb(26,26,26); \
                                       border-radius: 6px; \
                                       color: rgb(0,0,0); \
                                       background: rgb(189,0,0); \
                                   }";

static const QString buttonNone =
    "QPushButton \
                                   { \
                                       border: 2px solid rgb(26,26,26); \
                                       border-radius: 6px; \
                                       color: rgb(0,0,0); \
                                       background: rgb(180,180,180); \
                                   }";

const int WidgetsInAppearence{2};

GroupWidget::GroupWidget(int groupAddr, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::GroupWidget),
      m_widgetLayout(new QGridLayout()),
      m_selfAddr(groupAddr) {
  ui->setupUi(this);
  m_widgetLayout = new QGridLayout(ui->devicesTable);
  m_widgetLayout->setMargin(0);
  m_widgetLayout->setSpacing(10);
  m_widgetLayout->setContentsMargins(20, 0, 0, 0);
  m_widgetLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_widgetLayout->setAlignment(Qt::AlignLeft);
  ui->devicesTable->setLayout(m_widgetLayout);

  ui->hideButton->setCheckable(true);
  ui->hideButton->setChecked(false);
  ui->hideButton->setIconSize(QSize(10, 10));
  QIcon icon1;
  icon1.addFile(QString::fromUtf8(":/resources/images/hidecontrols-icon.png"),
                QSize(), QIcon::Normal, QIcon::Off);
  icon1.addFile(QString::fromUtf8(":/resources/images/showcontrols-icon.png"),
                QSize(), QIcon::Normal, QIcon::On);
  ui->hideButton->setIcon(icon1);
  ui->hideButton->setObjectName(QString::fromUtf8("hideControlButton"));
  ui->hideButton->setMinimumSize(QSize(112, 37));
  ui->hideButton->setMaximumSize(QSize(112, 37));
  QFont font1;
  font1.setFamily(QString::fromUtf8("Share Tech Mono"));
  ui->hideButton->setFont(font1);

  InLineEdit *name = new InLineEdit(m_selfAddr, false);
  ui->nameTable->addWidget(name);
  ui->nameTable->setAlignment(Qt::AlignmentFlag::AlignLeft);
  connect(name, &InLineEdit::nameEdited, this,
          [this](QString name) { m_name = name; });

  ui->statusButton->setIconSize(QSize(10, 10));
  QIcon icon2;
  icon2.addFile(QString::fromUtf8(":/resources/images/hidecontrols-icon.png"),
                QSize(), QIcon::Normal, QIcon::Off);
  icon2.addFile(QString::fromUtf8(":/resources/images/showcontrols-icon.png"),
                QSize(), QIcon::Normal, QIcon::On);
  ui->statusButton->setIcon(icon2);
  ui->statusButton->setMinimumSize(QSize(112, 37));
  ui->statusButton->setMaximumSize(QSize(112, 37));

  connect(ui->startButton, &QPushButton::clicked, this,
          &GroupWidget::startDevices);
  connect(ui->stopButton, &QPushButton::clicked, this,
          &GroupWidget::stopDevices);
  connect(ui->hideButton, &QPushButton::clicked, this,
          &GroupWidget::hideDevices);
  connect(ui->statusButton, &QPushButton::clicked, this,
          &GroupWidget::showStatus);
}

GroupWidget::~GroupWidget() { delete ui; }

void GroupWidget::addGroupMember(QPointer<DeviceWidget> member) {
  //  if (member == this) return;
  //  m_widgetLayout->addWidget(member);
  m_groupWidgets.append(member);
  resizeWidget();
  m_addresses.insert(member->getAddress());
  auto status = DeviceStatusGroup();
  status.errors = QStringList();
  status.devStarted = QMap<QString, bool>();
  m_status.insert(member->getAddress(), status);
}

void GroupWidget::removeGroupMember(QPointer<DeviceWidget> member) {
  //  if (member == this) return;
  m_widgetLayout->removeWidget(member);
  m_groupWidgets.removeOne(member);
  m_addresses.remove(member->getAddress());
  m_status.remove(member->getAddress());
}

const QSet<quint8> GroupWidget::getAddresses() { return m_addresses; }

void GroupWidget::startDevices() {
  auto command = model::events::network::CommandType::kStartDevices;
  emit groupEvent(
      GroupCommandFactory::createGroupCommand(m_addresses, command));
  ui->startButton->setStyleSheet(buttonOn);
  ui->stopButton->setStyleSheet(buttonNone);
}

void GroupWidget::stopDevices() {
  auto command = model::events::network::CommandType::kStopDevices;
  emit groupEvent(
      GroupCommandFactory::createGroupCommand(m_addresses, command));
  ui->startButton->setStyleSheet(buttonNone);
  ui->stopButton->setStyleSheet(buttonOff);
}

void GroupWidget::resizeWidget() {
  int maxWidth{-1};
  int widgetsCounter{0};
  int totalHeightInAppearence{0};
  for (auto widget : qAsConst(m_groupWidgets)) {
    m_widgetLayout->removeWidget(widget);
    if (widget->width() > maxWidth) {
      maxWidth = widget->width();
    }
  }
  std::sort(m_groupWidgets.begin(), m_groupWidgets.end(),
            [](DeviceWidget *a, DeviceWidget *b) {
              return a->getAddress() < b->getAddress();
            });
  for (auto widget : qAsConst(m_groupWidgets)) {
    widget->setConstraint(true);
    widget->setMinimumSize(maxWidth, widget->height());

    if (widgetsCounter < WidgetsInAppearence) {
      ++widgetsCounter;
      totalHeightInAppearence += widget->height();
      if (widgetsCounter > 0)
        totalHeightInAppearence += m_widgetLayout->spacing();
    }
    m_widgetLayout->addWidget(widget);
  }

  auto newSize = ui->devicesTable->size();
  int diffWidth = maxWidth - newSize.width();
  int diffHeight = totalHeightInAppearence - ui->devicesTable->height();

  newSize.rwidth() = maxWidth;

  if (diffHeight > 0) newSize.rheight() += diffHeight;
  ui->devicesTable->resize(newSize);

  auto winSize = size();
  winSize.rwidth() += diffWidth;
  winSize.rheight() += (diffHeight > 0) ? diffHeight : 0;
  resize(winSize);
}

void GroupWidget::hideDevices(bool flag) {
  m_hideDevices = flag;

  if (!m_hideDevices) {
    ui->hideButton->setText("Hide Devices");
    ui->devicesTable->setVisible(true);
    this->layout()->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
    this->adjustSize();
  } else {
    ui->hideButton->setText("Show Devices");
    ui->devicesTable->setVisible(false);
    this->layout()->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
    this->adjustSize();
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
  if (desc.isNull()) return;
  if (desc.data()->errors.has_value()) {
    m_status[addr].errors->append(desc.data()->errors.value());
    m_status[addr].errors->removeDuplicates();
  }
  if (desc.data()->devStarted.has_value()) {
    for (const auto &key : desc.data()->devStarted->keys()) {
      m_status[addr].devStarted->insert(key,
                                        desc.data()->devStarted->value(key));
    }
  }
  emit statusChanged(m_status);
}

const QString GroupWidget::getName() {
  if (m_name.isEmpty())
    return QString("Group %1").arg(m_selfAddr);
  else
    return m_name;
}

int GroupWidget::getGroupAddress() { return m_selfAddr; }
void GroupWidget::linkStatusChanged(int addr, bool status) {
  if (!status) {
    ui->linkLabel->setStyleSheet(
        "QLabel { \
                                       background: rgb(175,0,0); \
                                       border: 1px solid rgb(26,26,26); \
                                       border-radius: 3px; \
                               }");
  } else {
    ui->linkLabel->setStyleSheet(
        "QLabel { \
                                 background: rgb(0,102,51); \
                                 border: 1px solid rgb(26,26,26); \
                                 border-radius: 3px; \
                         }");
  }
  emit linkChanged(addr, status);
}

void GroupWidget::showStatus() {
  GroupStatusDialog *dialog = new GroupStatusDialog(this);
  for (const auto &device : m_groupWidgets) {
    dialog->addDevice(device->getAddress(), device->getName(),
                      device->getModel());
  }
  dialog->setModal(false);
  dialog->show();
  connect(this, &GroupWidget::statusChanged, dialog,
          &GroupStatusDialog::setStatus);
  connect(this, &GroupWidget::linkChanged, dialog,
          &GroupStatusDialog::deviceLinkChanged);
}
