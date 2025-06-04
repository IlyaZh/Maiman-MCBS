#include "groupwidgetfolded.h"

#include "ui_groupwidgetfolded.h"

using namespace StyleStorage::Group;

GroupWidgetFolded::GroupWidgetFolded(QWidget *parent)
    : QWidget(parent), ui(new Ui::GroupWidgetFolded) {
  ui->setupUi(this);
  this->setObjectName("GroupWidgetFolded");
  setAttribute(Qt::WA_StyledBackground, true);
  m_warning = new WarningWidget(true);
  ui->launchTable->addWidget(m_warning, Qt::AlignLeft);
  m_showButton = new GroupHideButton(this);
  m_showButton->setText(" " + tr("Show group"));
  ui->statusButtontable->insertWidget(1, m_showButton);
  ui->statusButtontable->setAlignment(Qt::AlignLeft);
  connect(ui->startButton, &QPushButton::clicked, this,
          &GroupWidgetFolded::startAll);
  connect(ui->stopButton, &QPushButton::clicked, this,
          &GroupWidgetFolded::stopAll);
  connect(m_showButton, &QPushButton::clicked, this,
          &GroupWidgetFolded::showWidget);
  GroupWidgetFolded::updateStyle();
}

GroupWidgetFolded::~GroupWidgetFolded() { delete ui; }

void GroupWidgetFolded::linkStatusChanged(bool status) {
  if (!status) {
    ui->linkLabel->setStyleSheet(StyleStorage::Device::linkDisconnected());
  } else {
    ui->linkLabel->setStyleSheet(StyleStorage::Device::linkConnected());
  }
}

void GroupWidgetFolded::setName(QString name) { ui->labelName->setText(name); }

void GroupWidgetFolded::addDevicesData(
    QMap<quint8, QSharedPointer<DeviceStatusGroup>> &status) {
  m_warning->addDevicesData(status);
}

void GroupWidgetFolded::updateStyle() {
  this->setStyleSheet(Widget::groupFoldedWidget());
  this->update();
  QList<QWidget *> widgets = this->findChildren<QWidget *>();
  for (QWidget *widget : widgets) {
    GuiWidgetInterface *interfaceWidget =
        dynamic_cast<GuiWidgetInterface *>(widget);
    if (interfaceWidget) {
      interfaceWidget->updateStyle();
    }
  }
}
