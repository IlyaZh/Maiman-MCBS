#include "devicewidget.h"

#include <QPainter>
#include <QStyleOption>
#include <algorithm>

#include "device/devicecommand.h"
#include "model/device/HiddenWidget.h"
#include "model/device/devicecondition.h"
#include "ui_devicewidget.h"
#include "widgets/binarywidget.h"
#include "widgets/controlwidget.h"
#include "widgets/inlineedit.h"
#include "widgets/readparameterfactory.h"
#include "widgets/readparameterwidget.h"

DeviceWidget::DeviceWidget(const QVector<HiddenWidget*>& widgets,
                           DeviceCondition* deviceCondition, QWidget* parent)
    : QWidget(parent),
      ui(new Ui::DeviceWidget),
      m_widgets(widgets),
      m_widgetLayout(new QGridLayout()),
      m_deviceAddress(new InLineEdit(0)) {
  ui->setupUi(this);
  QFont font14("Poppins Medium", 14);
  font14.setLetterSpacing(QFont::PercentageSpacing, 105);
  font14.setPixelSize(14);
  ui->modelLabel->setFont(font14);
  deviceCondition->bindLabel(ui->conditionLabel);
  deviceCondition->setParent(this);
  // Инициализация кнопки (Hide controls)
  auto m_hideControlsButton = new DeviceHideButton(this);
  m_hideControlsButton->setText(" " + tr("Minimize"));
  connect(m_hideControlsButton, &QPushButton::clicked, this,
          &DeviceWidget::hideWidget);
  ui->hideControlsLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);
  ui->hideControlsLayout->addWidget(m_hideControlsButton, Qt::AlignRight);
  for (int i = 0; i < m_widgets.size(); i++) {
    if (m_widgets.at(i)->whichWWidgetInside() == Control) {
      m_widgetLayout->addWidget(m_widgets.at(i), 1, i, Qt::AlignBottom);
    } else {
      m_widgetLayout->addWidget(m_widgets.at(i), 1, i, Qt::AlignTop);
    }

    if (m_widgets.at(i)->isPinned()) m_fixedWidgets++;
    QList<QWidget*> widgets = m_widgets.at(i)->findChildren<QWidget*>();
    for (QWidget* widget : widgets) {
      ControlWidget* controlWidget = dynamic_cast<ControlWidget*>(widget);
      if (controlWidget) {
        m_fixedEndWidgets++;
      }
    }
  }

  // Инициализация кнопок pin
  for (int i = m_fixedWidgets; i < m_fixedEndWidgets; ++i) {
    auto pinButton = new DevicePinButton(this);
    m_pinButtons.append(pinButton);
    m_widgetLayout->addWidget(pinButton, 0, i, Qt::AlignRight);
    connect(pinButton, &QPushButton::clicked, this,
            [i, this](bool checked) { pinButtonClicked(i, checked); });
  }

  m_widgetLayout->setContentsMargins(0, 0, 0, 0);
  m_widgetLayout->setSpacing(0);
  // m_widgetLayout->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
  ui->widgetBox->setLayout(m_widgetLayout);
  m_hideControlsButton->setVisible(!m_widgets.isEmpty());
  ui->gridLayout->addWidget(m_deviceAddress, 0, 1);
  connect(m_deviceAddress, &InLineEdit::nameEdited, this,
          &DeviceWidget::nameEdited);
  DeviceWidget::updateStyle();
  adjust();
  // TODO:: label_2, для отображения строки состояния драйвера, команда 0700
}

DeviceWidget::~DeviceWidget() { delete ui; }

void DeviceWidget::setAddress(int addr) { m_deviceAddress->setAddress(addr); }
QString DeviceWidget::getName() const { return m_deviceAddress->text(); }

void DeviceWidget::setLink(bool link) {
  ui->linkLabel->setStyleSheet(link ? StyleStorage::Device::linkConnected()
                                    : StyleStorage::Device::linkDisconnected());
  m_deviceAddress->setLink(link);
}

// private methods

void DeviceWidget::paintEvent(QPaintEvent*) {
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void DeviceWidget::adjust() {
  ui->widgetBox->adjustSize();
  this->adjustSize();
  this->setMinimumSize(this->size());
}

void DeviceWidget::setConstraint(bool state) {
  if (state)
    this->layout()->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
  else
    this->layout()->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
}

// private slots

void DeviceWidget::pinButtonClicked(int idx, bool state) {
  if (idx < m_widgets.count()) {
    auto widget = m_widgets.at(idx);
    widget->setPinned(state);
  }
}

void DeviceWidget::updateStyle() {
  ui->modelLabel->setStyleSheet(StyleStorage::Device::model());
  this->setStyleSheet(StyleStorage::Device::widget());
  this->update();
  QList<QWidget*> widgets = this->findChildren<QWidget*>();
  for (QWidget* widget : widgets) {
    GuiWidgetInterface* interfaceWidget =
        dynamic_cast<GuiWidgetInterface*>(widget);
    if (interfaceWidget) {
      interfaceWidget->updateStyle();
    }
  }
}

void DeviceWidget::setButton(QPointer<ButtonWidget> pButton) {
  ui->buttonsLayout->setContentsMargins(0, 0, 0, 0);
  ui->buttonsLayout->addWidget(pButton);
  ui->buttonsLayout->setAlignment(pButton, Qt::AlignBottom);
}

void DeviceWidget::setModel(QString model) {
  ui->modelLabel->setText(QString("Model: %1").arg(model));
}
