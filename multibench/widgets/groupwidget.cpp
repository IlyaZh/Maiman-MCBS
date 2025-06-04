#include "groupwidget.h"

#include "group/groupcommandfactory.h"
#include "ui_groupwidget.h"

using namespace StyleStorage::Group;

const int WidgetsInAppearence{2};

GroupWidget::GroupWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::GroupWidget),
      m_widgetLayout(new QGridLayout()) {
  ui->setupUi(this);
  this->setObjectName("GroupWidget");
  m_warning = new WarningWidget(true);
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

  m_statusButton->setChecked(true);
  m_hideButton->setText(" " + tr("Hide group"));
  m_statusButton->setText(" " + tr("Minimize all"));
  ui->statusButtontable->insertWidget(1, m_hideButton);
  ui->statusButtontable->insertWidget(2, m_statusButton);
  ui->statusButtontable->setAlignment(Qt::AlignLeft);
  //  m_name = new InLineEdit(m_selfAddr, false);
  //  ui->nameTable->insertWidget(1, m_name, Qt::AlignmentFlag::AlignLeft);
  ui->nameTable->setAlignment(Qt::AlignmentFlag::AlignLeft);

  connect(ui->startButton, &QPushButton::clicked, this, &GroupWidget::startAll);
  connect(ui->stopButton, &QPushButton::clicked, this, &GroupWidget::stopAll);
  connect(m_hideButton, &QPushButton::clicked, this, &GroupWidget::hideWidget);
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
  isHiddenWidget(true);
  qDebug() << "add new member" << member->size() << member->sizeHint();
  emit closeGroupStatusDialog();
  connect(member, &DeviceHolder::hideStatus, this,
          &GroupWidget::isHiddenWidget);
}

void GroupWidget::removeGroupMember(QPointer<DeviceHolder> member) {
  //  if (member == this) return;
  emit closeGroupStatusDialog();
  m_widgetLayout->removeWidget(member);
  m_groupWidgets.removeOne(member);
  disconnect(member, &DeviceHolder::hideStatus, this,
             &GroupWidget::isHiddenWidget);
  isHiddenWidget(true);
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
    m_hideButton->setText(" " + tr("Hide group"));
    m_statusButton->setVisible(true);
    ui->devicesTable->setVisible(true);
    //    this->layout()->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
    setMinimumHeight(0);
    setMaximumHeight(QWIDGETSIZE_MAX);
    //    this->adjustSize();
    this->setMinimumWidth(this->size().rwidth());
    updateGeometry();
  } else {
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_hideButton->setText(" " + tr("Show group"));
    m_statusButton->setVisible(false);
    ui->devicesTable->setVisible(false);
    setMinimumHeight(110);
    setMaximumHeight(110);
    this->setMinimumWidth(this->size().rwidth());
    resize(width(), 110);
    updateGeometry();
  }
  // resizeWidget();
}

void GroupWidget::resizeEvent(QResizeEvent *event) {
  qDebug() << "GroupWidget resized to" << event->size();
  QWidget::resizeEvent(event);
}
QSize GroupWidget::sizeHint() const {
  if (m_hideDevices) {
    return QSize(QWidget::sizeHint().width(), 110);
  } else {
    return QWidget::sizeHint();  // автоматическое подстраивание под содержимое
  }
}

QSize GroupWidget::minimumSizeHint() const {
  if (m_hideDevices) {
    return QSize(QWidget::minimumSizeHint().width(), 110);
  } else {
    return QWidget::minimumSizeHint();
  }
}
void GroupWidget::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void GroupWidget::addDevicesData(
    QMap<quint8, QSharedPointer<DeviceStatusGroup>> &status) {
  m_warning->addDevicesData(status);
}

void GroupWidget::setName(QString name) { ui->labelName->setText(name); }

void GroupWidget::linkStatusChanged(bool status) {
  if (!status) {
    ui->linkLabel->setStyleSheet(StyleStorage::Device::linkDisconnected());
  } else {
    ui->linkLabel->setStyleSheet(StyleStorage::Device::linkConnected());
  }
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
  //  if (m_allStarted) {
  //    ui->startButton->setStyleSheet(Widget::buttonLaunched());
  //    ui->stopButton->setStyleSheet(Widget::buttonInMiddle());
  //  } else {
  //    ui->startButton->setStyleSheet(Widget::buttonInMiddle());
  //    ui->stopButton->setStyleSheet(Widget::buttonStopped());
  //  }
}

bool GroupWidget::findHiddenDevices() {
  m_statusButton->setChecked(true);
  bool isHidden = true;
  for (auto device : qAsConst(m_groupWidgets)) {
    isHidden &= device->isHide();
    //    if (device->isHide()) {
    //      isHidden = true;
    //    }
  }
  if (isHidden) {
    m_statusButton->setText(" " + tr("Maximize all"));
  } else {
    m_statusButton->setText(" " + tr("Minimize all"));
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
  emit sizeChanged();
}
