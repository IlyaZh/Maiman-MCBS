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

GroupWidget::GroupWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::GroupWidget),
      m_widgetLayout(new QGridLayout()) {
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
  ui->hideButton->setMinimumSize(QSize(125, 37));
  ui->hideButton->setMaximumSize(QSize(125, 37));
  QFont font1;
  font1.setFamily(QString::fromUtf8("Share Tech Mono"));
  ui->hideButton->setFont(font1);

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
    if (widget->width() > maxWidth) {
      maxWidth = widget->width();
    }
  }

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

  if (m_hideDevices) {
    ui->hideButton->setText("Show");
    ui->devicesTable->setVisible(false);
  } else {
    ui->hideButton->setText("Hide");
    ui->devicesTable->setVisible(true);
  }

  resizeWidget();
}

void GroupWidget::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void GroupWidget::setDevicesStatus(
    quint8 addr, std::optional<QSharedPointer<DeviceStatusGroup>> &desc) {
  if (desc == std::nullopt) return;
  if (desc->data()->errors.has_value()) {
    m_status[addr].errors->append(desc->data()->errors.value());
    m_status[addr].errors->removeDuplicates();
  }
  if (desc->data()->devStarted.has_value()) {
    for (auto key : desc->data()->devStarted->keys()) {
      if (!m_status[addr].devStarted->contains(key))
        m_status[addr].devStarted->insert(key,
                                          desc->data()->devStarted->value(key));
    }
  }
}

void GroupWidget::showStatus() {
  for (auto status : m_status) {
    qDebug() << "status" << status.errors.value().join("; ");
    for (auto key : status.devStarted.value().keys()) {
      qDebug() << key << status.devStarted.value().value(key);
    }
  }
}
