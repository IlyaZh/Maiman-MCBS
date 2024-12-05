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

const QString DeviceWidget::linkStyleOn =
    "QLabel { \
    background: #39CA76;\
    border: 0px solid #1A1A1A;\
    border-radius: 3px;\
}";

const QString DeviceWidget::linkStyleOff =
    "QLabel { \
    background: #FF403A;\
    border: 1px solid #1A1A1A;\
    border-radius: 3px;\
}";

const QString DeviceWidget::labelEnableStyle =
    "QLabel { \
    color: #FFFFFF;\
    background-color: #282828;\
}";

const QString DeviceWidget::labelDisableStyle =
    "QLabel { \
    background-color: #282828;\
    color: rgb(153,153,153); \
}";

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

DeviceWidget::DeviceWidget(
    const DeviceWidgetDesc& description,
    const QMap<quint16, QSharedPointer<DevCommand>>& commands,
    const QMap<quint16, QSharedPointer<CommandConverter>>& converters,
    QWidget* parent)
    : QWidget(parent),
      ui(new Ui::DeviceWidget),
      m_buttons(description.buttons),
      m_commands(commands),
      m_converters(converters),
      m_widgetLayout(new QGridLayout()),
      m_deviceAddress(new InLineEdit(0)) {
  ui->setupUi(this);
  QFont font14("Poppins Medium", 14);
  font14.setLetterSpacing(QFont::PercentageSpacing, 105);
  font14.setPixelSize(14);
  ui->modelLabel->setText(QString("Model: %1").arg(description.name));
  ui->modelLabel->setFont(font14);
  m_id = description.id;

  // Инициализация кнопки (Hide controls)
  auto m_hideControlsButton = new DeviceHideButton(this);
  m_hideControlsButton->setText(" " + tr("Hide controls"));
  connect(m_hideControlsButton, &QPushButton::clicked, this,
          &DeviceWidget::hideControlsButtonClicked);
  m_widgetLayout->addWidget(m_hideControlsButton, 0, 0, Qt::AlignLeft);

  // Инициализация виджетов
  QVector<ReadParameterWidget*> readOnlyWidgets;
  for (const auto& control : description.controls) {
    auto valueCmd = m_commands.value(control.value, nullptr);
    auto valueConverter = m_converters.value(control.value, nullptr);
    auto maxCmd = m_commands.value(control.max, nullptr);
    auto maxConverter = m_converters.value(control.max, nullptr);
    auto minCmd = m_commands.value(control.min, nullptr);
    auto minConverter = m_converters.value(control.min, nullptr);
    auto realCmd = m_commands.value(control.real, nullptr);
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
      if (control.fixed) ++m_fixedWidgets;
      hiddenWidget->layout()->setContentsMargins(10, 0, 0, 0);
      m_widgetLayout->addWidget(hiddenWidget, 1, m_widgets.size());
      hiddenWidget->addWidget(widget);
      if (control.name == "current") hiddenWidget->setPinned(true);
      m_widgets.append(hiddenWidget);
      m_widgetsTable.insert(widget);
      //      for (auto code : widget->Subscribe()) {
      //        m_widgetsTable.insert(code, widget);
      //      }
    }
  }
  // Закидываем неизменяемые параметры в виджет
  if (readOnlyWidgets.count() > 0) {
    auto hiddenWidget = new HiddenWidget(this);
    hiddenWidget->layout()->setContentsMargins(10, 16, 10, 0);
    auto maxUnitsLengthIt = *std::max_element(
        std::begin(readOnlyWidgets), std::end(readOnlyWidgets),
        [=](ReadParameterWidget* widgetA, ReadParameterWidget* widgetB) {
          return widgetA->getUnitslength() < widgetB->getUnitslength();
        });
    for (auto item : qAsConst(readOnlyWidgets)) {
      item->setContentsMargins(0, 0, 0, 0);
      item->setUnitsLength(maxUnitsLengthIt->getUnitslength());
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
      hiddenWidget->layout()->setContentsMargins(10, 16, 10, 0);
    }
    auto cmd = m_commands.value(item.code, nullptr);
    auto converter = m_converters.value(item.code, nullptr);
    if (cmd) {
      auto binaryWidget = new BinaryWidget(item, converter, hiddenWidget);
      binaryWidget->setContentsMargins(10, 2, 10, 0);
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
        m_commands.contains(button.code)) {
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

void DeviceWidget::setAddress(int addr) {
  m_address = addr;
  m_deviceAddress->setAddress(addr);
}

int DeviceWidget::getAddress() const { return m_address; }
int DeviceWidget::getId() const { return m_id; }
QString DeviceWidget::getName() const { return m_deviceAddress->text(); }
QString DeviceWidget::getModel() const { return ui->modelLabel->text(); }

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

void DeviceWidget::setLaserButton(quint16 value) {
  for (const auto& button : qAsConst(m_buttons)) {
    if (button.name == "Laser") {
      m_laserButton->setData(button.code, value);
    }
  }
}

void DeviceWidget::setTecButton(quint16 value) {
  for (const auto& button : qAsConst(m_buttons)) {
    if (button.name == "TEC") {
      m_laserButton->setData(button.code, value);
    }
  }
}

void DeviceWidget::laserButtonClicked() {
  auto search =
      std::find_if(m_buttons.begin(), m_buttons.end(),
                   [](const auto& button) { return (button.name == "Laser"); });
  if (search != m_buttons.end()) {
    auto laserButton = search.value();
    auto cmd = m_commands.value(laserButton.code, 0);
    if (cmd) {
      cmd->setFromWidget((cmd->valueInt() & laserButton.mask)
                             ? laserButton.offCommand
                             : laserButton.onCommand);
    }
  }
}

void DeviceWidget::tecButtonClicked() {
  auto search =
      std::find_if(m_buttons.begin(), m_buttons.end(),
                   [](const auto& button) { return (button.name == "TEC"); });
  if (search != m_buttons.end()) {
    auto laserButton = search.value();
    auto cmd = m_commands.value(laserButton.code, 0);
    if (cmd) {
      cmd->setFromWidget((cmd->valueInt() & laserButton.mask)
                             ? laserButton.offCommand
                             : laserButton.onCommand);
    }
  }
}

void DeviceWidget::hideControlsButtonClicked(bool flag) {
  m_hideControls = flag;

  for (int idx = m_fixedWidgets; idx < m_widgets.count(); ++idx) {
    int pinShift = idx - 1;
    auto widget = m_widgets.at(idx);
    auto pinButton = m_pinButtons.value(pinShift, nullptr);
    auto topMargin = widget->layout()->contentsMargins().top();
    auto bottomMargin = widget->layout()->contentsMargins().bottom();
    if (m_hideControls) {
      if (!widget->isPinned()) {
        setConstraint(false);
        widget->setShown(false);
        widget->layout()->setContentsMargins(0, topMargin, 0, bottomMargin);
        widget->layout()->setSpacing(0);
        if (pinButton != nullptr) pinButton->setVisible(false);
      }
    } else {
      if (!widget->isShown()) {
        setConstraint(true);
        widget->setShown(true);
        widget->layout()->setContentsMargins(10, topMargin, 10, bottomMargin);
        widget->layout()->setSpacing(10);
        if (pinButton != nullptr) pinButton->setVisible(true);
      }
    }
  }
  adjust();
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
    adjust();
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
