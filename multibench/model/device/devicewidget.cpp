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

DeviceWidget::DeviceWidget(
    const DeviceWidgetDesc& description,
    const QMap<quint16, QSharedPointer<CommandConverter>>& converters,
    QWidget* parent)
    : QWidget(parent),
      ui(new Ui::DeviceWidget),
      m_buttons(description.buttons),
      m_converters(converters),
      m_widgetLayout(new QGridLayout()),
      m_deviceAddress(new InLineEdit(0)) {
  ui->setupUi(this);
  QFont font14("Poppins Medium", 14);
  font14.setLetterSpacing(QFont::PercentageSpacing, 105);
  font14.setPixelSize(14);
  ui->modelLabel->setText(QString("Model: %1").arg(description.name));
  ui->modelLabel->setFont(font14);

  // Инициализация кнопки (Hide controls)
  auto m_hideControlsButton = new DeviceHideButton(this);
  m_hideControlsButton->setText(" " + tr("Hide controls"));
  connect(m_hideControlsButton, &QPushButton::clicked, this,
          &DeviceWidget::hideControlsButtonClicked);
  m_widgetLayout->addWidget(m_hideControlsButton, 0, 0, Qt::AlignLeft);

  // Инициализация виджетов
  QVector<ReadParameterWidget*> readOnlyWidgets;
  for (const auto& control : description.controls) {
    auto valueConverter = m_converters.value(control.value, nullptr);
    auto maxConverter = m_converters.value(control.max, nullptr);
    auto minConverter = m_converters.value(control.min, nullptr);
    auto realConverter = m_converters.value(control.real, nullptr);

    if (realConverter != nullptr and valueConverter == nullptr) {
      // Обработка неизменяемых параметров
      readOnlyWidgets.append(ReadParameterFactory::createReadParameter(
          control.name, realConverter));
    } else {
      // Обработка изменяемых параметров
      auto hiddenWidget = new HiddenWidget(this);
      auto widget =
          new ControlWidget(control.name, valueConverter, maxConverter,
                            minConverter, realConverter, hiddenWidget);
      hiddenWidget->setMargins(10, 0, 10, 0);
      m_widgetLayout->addWidget(hiddenWidget, 1, m_widgets.size());
      hiddenWidget->addWidget(widget);
      if (control.fixed) {
        ++m_fixedWidgets;
        hiddenWidget->setPinned(true);
      }
      m_pinnedWidgets.insert(m_widgets.size(), widget->getName());
      m_widgets.append(hiddenWidget);
      m_widgetsTable.insert(widget);
    }
  }
  // Закидываем неизменяемые параметры в виджет
  if (readOnlyWidgets.count() > 0) {
    auto hiddenWidget = new HiddenWidget(this);
    hiddenWidget->setMargins(10, 16, 10, 0);
    auto maxUnitsLengthIt = *std::max_element(
        std::begin(readOnlyWidgets), std::end(readOnlyWidgets),
        [=](ReadParameterWidget* widgetA, ReadParameterWidget* widgetB) {
          return widgetA->getUnitslength() < widgetB->getUnitslength();
        });
    for (auto item : qAsConst(readOnlyWidgets)) {
      item->setUnitsLength(maxUnitsLengthIt->getUnitslength());
      item->setContentsMargins(0, 0, 0, 0);
      hiddenWidget->addWidget(item);
      m_widgetsTable.insert(item);
      //      for (auto code : item->Subscribe()) {
      //        m_widgetsTable.insert(code, item);
      //      }
    }
    readOnlyWidgets.clear();
    m_widgetLayout->addWidget(hiddenWidget, 1, m_widgets.size(), Qt::AlignTop);
    m_widgets.append(hiddenWidget);
  }

  // Инициализация checkbox'ов
  QPointer<HiddenWidget> hiddenWidget;
  for (const auto& item : qAsConst(description.checkboxes)) {
    if (!hiddenWidget) {
      hiddenWidget = new HiddenWidget(this);
      hiddenWidget->setMargins(10, 16, 10, 0);
    }
    auto converter = m_converters.value(item.code, nullptr);
    if (converter) {
      auto binaryWidget = new BinaryWidget(item, converter, hiddenWidget);
      binaryWidget->setContentsMargins(0, 0, 10, 0);
      hiddenWidget->addWidget(binaryWidget);
      m_widgetsTable.insert(binaryWidget);
      //      for (auto code : binaryWidget->Subscribe()) {
      //        m_widgetsTable.insert(code, binaryWidget);
      //      }
    }
  }
  if (hiddenWidget) {
    m_widgetLayout->addWidget(hiddenWidget.data(), 1, m_widgets.size(),
                              Qt::AlignTop);
    m_widgets.append(hiddenWidget);
  }

  // Инициализация кнопок pin
  for (int i = m_fixedWidgets; i < m_widgets.count(); ++i) {
    auto pinButton = new DevicePinButton(this);
    m_pinButtons.append(pinButton);
    m_widgetLayout->addWidget(pinButton, 0, i, Qt::AlignRight);
    connect(pinButton, &QPushButton::clicked, this,
            [i, this](bool checked) { pinButtonClicked(i, checked); });
  }

  // Инциализация кнопок Laser и TEC
  for (const auto& button : qAsConst(description.buttons)) {
    QPointer<ButtonWidget> pButton;
    if (button.name.compare("laser", Qt::CaseInsensitive) == 0 &&
        m_converters.contains(button.code)) {
      auto converter = m_converters.value(button.code);
      m_laserButton = new ButtonWidget("Laser", button, converter, this);
      pButton = m_laserButton;
      m_widgetsTable.insert(m_laserButton);
    } else if (button.name.compare("TEC", Qt::CaseInsensitive) == 0) {
      auto converter = m_converters.value(button.code);
      m_tecButton = new ButtonWidget("TEC", button, converter, this);
      pButton = m_tecButton;
      m_widgetsTable.insert(m_tecButton);
    }

    if (pButton) {
      ui->buttonsLayout->setContentsMargins(0, 0, 0, 0);
      ui->buttonsLayout->addWidget(pButton);
      ui->buttonsLayout->setAlignment(pButton, Qt::AlignBottom);
    }
  }
  //  m_widgetLayout->setAlignment(Qt::AlignTop);
  m_widgetLayout->setContentsMargins(0, 0, 0, 0);
  m_widgetLayout->setSpacing(0);
  // m_widgetLayout->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
  ui->widgetBox->setLayout(m_widgetLayout);
  m_hideControlsButton->setVisible(!m_widgets.isEmpty());

  m_deviceCondition = new DeviceCondition(m_converters, description.leds,
                                          ui->conditionLabel, this);
  m_widgetsTable.insert(m_deviceCondition);
  ui->gridLayout->addWidget(m_deviceAddress, 0, 2);
  connect(m_deviceAddress, &InLineEdit::nameEdited, this,
          &DeviceWidget::nameEdited);
  for (auto widget : qAsConst(m_widgetsTable)) {
    connect(widget, &GuiWidgetBase::setDataFromWidget, this,
            &DeviceWidget::acceptDataFromWidget);
  }
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

void DeviceWidget::hideControlsButtonClicked(bool flag) {
  m_hideControls = flag;
  QMap<QString, bool> widgets;
  for (int idx = m_fixedWidgets; idx < m_widgets.count(); ++idx) {
    auto widget = m_widgets.at(idx);
    if (widget->isPinned()) {
      widgets.insert(m_pinnedWidgets.value(idx), true);
    } else {
      widgets.insert(m_pinnedWidgets.value(idx), false);
    }
  }
  //  for (int idx = m_fixedWidgets; idx < m_widgets.count(); ++idx) {
  //    int pinShift = idx - 1;
  //    auto widget = m_widgets.at(idx);
  //    auto pinButton = m_pinButtons.value(pinShift, nullptr);
  //    if (m_hideControls) {
  //      if (!widget->isPinned()) {
  //        setConstraint(false);
  //        widget->setShown(false);
  //        if (pinButton != nullptr) pinButton->setVisible(false);
  //      }
  //    } else {
  //      if (!widget->isShown()) {
  //        setConstraint(true);
  //        widget->setShown(true);
  //        if (pinButton != nullptr) pinButton->setVisible(true);
  //      }
  //    }
  //  }
  //  adjust();
  emit hideWidget(widgets);
}

void DeviceWidget::pinButtonClicked(int idx, bool state) {
  if (idx < m_widgets.count()) {
    auto widget = m_widgets.at(idx);
    widget->setPinned(state);
  }
}

void DeviceWidget::updateValue(const model::Event& event) {
  if (std::holds_alternative<model::events::network::Answer>(event.data_)) {
    const auto& answer = std::get<model::events::network::Answer>(event.data_);
    for (auto widget : qAsConst(m_widgetsTable)) {
      if (widget->Subscribe().contains(answer.reg_)) {
        widget->setData(answer.reg_, answer.value_);
      }
    }
    //    adjust();
  } else if (std::holds_alternative<model::events::network::ChangeSystemStyle>(
                 event.data_)) {
    updateStyle();
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

void DeviceWidget::addGroupMember(QSharedPointer<GroupInterface> /*member*/) {
  std::runtime_error("Not implemented");
}

void DeviceWidget::removeGroupMember(
    QSharedPointer<GroupInterface> /*member*/) {
  std::runtime_error("Not implemented");
}
