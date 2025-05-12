#include "devicefoldedwidget.h"

#include "model/device/HiddenWidget.h"
#include "ui_devicefoldedwidget.h"
#include "widgets/buttonwidget.h"
#include "widgets/foldedcontrolwidget.h"

DeviceFoldedWidget::DeviceFoldedWidget(
    const QVector<FoldedControlWidget*>& foldedWidgets, QWidget* parent)
    : QWidget(parent),
      ui(new Ui::DeviceFoldedWidget),
      m_widgetLayout(new QHBoxLayout()),
      m_foldedWidgets(foldedWidgets) {
  ui->setupUi(this);
  setObjectName("DeviceFoldedWidget");
  setAttribute(Qt::WA_StyledBackground, true);
  //  ui->modelLabel->setText(description.name);
  m_warning = new WarningWidget();
  auto m_showControlsButton = new DeviceShowButton(this);
  m_showControlsButton->setText(" " + tr("Maximize"));
  connect(m_showControlsButton, &QPushButton::clicked, this,
          &DeviceFoldedWidget::showWidget);
  ui->showControlsLayout->addWidget(m_showControlsButton);
  ui->showControlsLayout->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  ui->buttonsLayout->setSpacing(12);
  ui->buttonsLayout->addWidget(m_warning);
  ui->buttonsLayout->setAlignment(m_warning, Qt::AlignVCenter | Qt::AlignRight);
  ui->widgetsLayout->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  ui->widgetsLayout->setContentsMargins(0, 0, 0, 0);
  m_widgetLayout->setContentsMargins(0, 0, 0, 0);
  m_widgetLayout->setSpacing(0);
  for (int i = 0; i < m_foldedWidgets.size(); i++) {
    m_widgetLayout->addWidget(m_foldedWidgets.at(i));
  }
  // m_widgetLayout->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
  ui->widgetsLayout->addLayout(m_widgetLayout);
  DeviceFoldedWidget::updateStyle();
}

DeviceFoldedWidget::~DeviceFoldedWidget() { delete ui; }

void DeviceFoldedWidget::setAddress(int addr) {
  ui->nameLabel->setText(QString("ID: %1").arg(addr));
}

void DeviceFoldedWidget::setLink(bool link) {
  ui->linkLabel->setStyleSheet(link ? StyleStorage::Device::linkConnected()
                                    : StyleStorage::Device::linkDisconnected());
  ui->nameLabel->setStyleSheet(
      link ? StyleStorage::FoldedDevice::labelNameConnected()
           : StyleStorage::FoldedDevice::labelNameDisconnected());
}

void DeviceFoldedWidget::updateStyle() {
  ui->modelLabel->setStyleSheet(StyleStorage::Device::model());
  //  ui->showButton->setStyleSheet(StyleStorage::Device::hideButton());
  this->setStyleSheet(StyleStorage::FoldedDevice::widget());
  this->update();
}

void DeviceFoldedWidget::setStatus(DeviceStatusGroup& status) {
  m_warning->addData(status);
}

void DeviceFoldedWidget::setButton(QPointer<ButtonWidget> pButton) {
  ui->buttonsLayout->setContentsMargins(0, 0, 0, 0);
  ui->buttonsLayout->addWidget(pButton);
  ui->buttonsLayout->setAlignment(pButton, Qt::AlignHCenter | Qt::AlignRight);
}

void DeviceFoldedWidget::setVisibleWidget() {
  for (auto& widget : m_foldedWidgets) {
    widget->setShown();
  }
}

void DeviceFoldedWidget::setModel(QString model) {
  ui->modelLabel->setText(model);
}
