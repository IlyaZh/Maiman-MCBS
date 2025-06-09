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
  m_showButton = new GroupHideButton(buttonClass::down, this);
  m_showButton->setChecked(true);
  m_showButton->setCheckable(false);
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
  QVector<bool> devs;
  for (const auto &data : qAsConst(status)) {
    devs.append(data->isStarted.value());
  }
  bool all = devs.at(0);
  for (int i = 1; i < devs.size(); i++) {
    all &= devs.at(i);
  }
  if (all) {
    ui->startButton->setStyleSheet(Widget::buttonLaunched());
    ui->stopButton->setStyleSheet(Widget::buttonInMiddle());
  } else {
    ui->startButton->setStyleSheet(Widget::buttonInMiddle());
    ui->stopButton->setStyleSheet(Widget::buttonStopped());
  }
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
