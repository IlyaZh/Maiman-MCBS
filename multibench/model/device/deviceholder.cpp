#include "deviceholder.h"

#include "device/devicecommand.h"
#include "model/device/HiddenWidget.h"
#include "model/device/devicecondition.h"
#include "ui_devicewidget.h"
#include "widgets/binarywidget.h"
#include "widgets/controlwidget.h"
#include "widgets/foldedcontrolwidget.h"
#include "widgets/inlineedit.h"
#include "widgets/readparameterfactory.h"
#include "widgets/readparameterwidget.h"

DeviceHolder::DeviceHolder(
    const DeviceWidgetDesc& description,
    const QMap<quint16, QSharedPointer<CommandConverter>>& converters,
    QWidget* parent)
    : QWidget(parent),
      m_description(description),
      m_converters(converters),
      m_widgetLayout(new QVBoxLayout()) {
  m_id = m_description.id;

  QVector<ReadParameterWidget*> readOnlyWidgets;
  for (const auto& control : m_description.controls) {
    auto valueConverter = m_converters.value(control.value, nullptr);
    auto maxConverter = m_converters.value(control.max, nullptr);
    auto minConverter = m_converters.value(control.min, nullptr);
    auto realConverter = m_converters.value(control.real, nullptr);

    if (realConverter != nullptr and valueConverter == nullptr) {
      readOnlyWidgets.append(ReadParameterFactory::createReadParameter(
          control.name, realConverter));
    } else {
      auto hiddenWidget = new HiddenWidget(this);
      auto widget =
          new ControlWidget(control.name, valueConverter, maxConverter,
                            minConverter, realConverter, hiddenWidget);
      hiddenWidget->setMargins(10, 0, 10, 0);
      hiddenWidget->addWidget(widget);
      auto foldedWidget = new FoldedControlWidget(control.name, valueConverter,
                                                  realConverter, this);
      foldedWidget->setContentsMargins(10, 0, 10, 0);
      if (control.fixed) {
        hiddenWidget->setPinned(true);
        foldedWidget->setPinned(true);
      }
      m_widgets.append(hiddenWidget);
      m_widgetsTable.insert(widget);
      m_foldedWidgets.append(foldedWidget);
      m_widgetsTable.insert(foldedWidget);
      connect(hiddenWidget, &HiddenWidget::pinned, foldedWidget,
              [foldedWidget](bool pin) { foldedWidget->setPinned(pin); });
    }
  }
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
    }
    m_widgets.append(hiddenWidget);
    readOnlyWidgets.clear();
  }
  QPointer<HiddenWidget> hiddenWidget;
  for (const auto& item : qAsConst(description.checkboxes)) {
    if (!hiddenWidget) {
      hiddenWidget = new HiddenWidget(this);
      hiddenWidget->setMargins(10, 16, 10, 0);
    }
    auto converter = m_converters.value(item.code, nullptr);
    if (converter) {
      auto binaryWidget = new BinaryWidget(item, converter, hiddenWidget);
      binaryWidget->setContentsMargins(0, 0, 0, 0);
      hiddenWidget->addWidget(binaryWidget);
      m_widgetsTable.insert(binaryWidget);
    }
  }
  if (hiddenWidget) {
    m_widgets.append(hiddenWidget);
  }
  m_deviceCondition = new DeviceCondition(m_converters, description.leds, this);
  m_widgetsTable.insert(m_deviceCondition);

  m_expandedWidget = new DeviceWidget(m_widgets, m_deviceCondition);
  m_foldedWidget = new DeviceFoldedWidget(m_foldedWidgets);
  m_expandedWidget->setModel(m_description.name);
  m_foldedWidget->setModel(m_description.name);

  for (const auto& button : qAsConst(description.buttons)) {
    QPointer<ButtonWidget> pButton;
    QPointer<ButtonWidget> pSmallButton;
    if (button.name.compare("laser", Qt::CaseInsensitive) == 0 &&
        m_converters.contains(button.code)) {
      auto converter = m_converters.value(button.code);
      pButton = new ButtonWidget("Laser", button, converter, this);
      m_buttonWidgets.append(pButton);
      m_widgetsTable.insert(pButton);
      pSmallButton = new ButtonWidget("Laser", button, converter, this);
      pSmallButton->setSize(true);
      m_buttonWidgets.append(pSmallButton);
      m_widgetsTable.insert(pSmallButton);
    } else if (button.name.compare("TEC", Qt::CaseInsensitive) == 0) {
      auto converter = m_converters.value(button.code);
      pButton = new ButtonWidget("TEC", button, converter, this);
      m_buttonWidgets.append(pButton);
      m_widgetsTable.insert(pButton);
      pSmallButton = new ButtonWidget("TEC", button, converter, this);
      pSmallButton->setSize(true);
      m_buttonWidgets.append(pSmallButton);
      m_widgetsTable.insert(pSmallButton);
    }
    if (pButton) {
      m_expandedWidget->setButton(pButton);
      m_foldedWidget->setButton(pSmallButton);
    }
  }
  this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
  for (auto widget : qAsConst(m_widgetsTable)) {
    connect(widget, &GuiWidgetBase::setDataFromWidget, this,
            &DeviceHolder::acceptDataFromWidget);
  }
  m_widgetLayout->setMargin(0);
  m_widgetLayout->setSpacing(0);
  m_widgetLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  m_widgetLayout->addWidget(m_expandedWidget);
  m_widgetLayout->addWidget(m_foldedWidget);
  m_foldedWidget->hide();
  this->setLayout(m_widgetLayout);
  connect(m_expandedWidget, &DeviceWidget::acceptDataFromWidget, this,
          &DeviceHolder::acceptDataFromWidget);
  connect(m_foldedWidget, &DeviceFoldedWidget::acceptDataFromWidget, this,
          &DeviceHolder::acceptDataFromWidget);
  connect(m_expandedWidget, &DeviceWidget::hideWidget, this,
          &DeviceHolder::hideControlsButtonClicked);
  connect(m_foldedWidget, &DeviceFoldedWidget::showWidget, this,
          &DeviceHolder::showWidgetButtonClicked);
  connect(m_expandedWidget, &DeviceWidget::nameEdited, this,
          [this](QString name, int addr) {
            m_name = name;
            emit nameEdited(name, addr);
          });
  connect(this, &DeviceHolder::statusChanged, m_foldedWidget,
          &DeviceFoldedWidget::setStatus);
  m_status.errors = QStringList();
  m_status.interlocks = QStringList();
}

DeviceHolder::~DeviceHolder() {}

void DeviceHolder::updateValue(const model::Event& event) {
  if (std::holds_alternative<model::events::network::Answer>(event.data_)) {
    const auto& answer = std::get<model::events::network::Answer>(event.data_);
    for (auto widget : qAsConst(m_widgetsTable)) {
      if (widget->Subscribe().contains(answer.reg_)) {
        widget->setData(answer.reg_, answer.value_);
      }
    }
  } else if (std::holds_alternative<model::events::network::ChangeSystemStyle>(
                 event.data_)) {
    updateStyle();
  }
}

void DeviceHolder::setAddress(int addr) {
  m_address = addr;
  m_expandedWidget->setAddress(m_address);
  m_foldedWidget->setAddress(m_address);
  m_name = m_expandedWidget->getName();
}

int DeviceHolder::getAddress() const { return m_address; }

int DeviceHolder::getId() const { return m_id; }
QString DeviceHolder::getName() const { return m_name; }
QString DeviceHolder::getModel() const { return m_description.name; }

void DeviceHolder::updateStyle() { m_expandedWidget->updateStyle(); }

void DeviceHolder::setLink(bool link) {
  m_expandedWidget->setLink(link);
  m_foldedWidget->setLink(link);
}
void DeviceHolder::setConstraint(bool state) {
  m_expandedWidget->setConstraint(state);
}

void DeviceHolder::hideControlsButtonClicked() {
  m_expandedWidget->hide();
  m_widgetSize = m_expandedWidget->size();
  m_foldedWidget->setMinimumWidth(m_widgetSize.rwidth());
  m_foldedWidget->setMaximumWidth(m_widgetSize.rwidth());
  m_foldedWidget->setVisibleWidget();
  m_foldedWidget->show();
  m_widgetLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
  //  this->adjustSize();
}
void DeviceHolder::showWidgetButtonClicked() {
  m_expandedWidget->setMinimumWidth(m_widgetSize.rwidth());
  m_expandedWidget->setMaximumWidth(m_widgetSize.rwidth());
  m_expandedWidget->show();
  m_foldedWidget->hide();
  m_widgetLayout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
  //  this->adjustSize();
}

void DeviceHolder::setDevicesStatus(quint8 addr,
                                    QSharedPointer<DeviceStatusGroup> desc) {
  if (addr != m_address) return;
  if (desc.isNull()) return;
  if (desc.data()->isError.has_value() and desc.data()->isError) {
    m_status.errors->clear();
    if (desc.data()->errors.has_value())
      m_status.errors->append(desc.data()->errors.value());
    m_status.errors->removeDuplicates();
  } else {
    m_status.errors->clear();
  }
  if (desc.data()->isInterlock.has_value() and desc.data()->isInterlock) {
    m_status.interlocks->clear();
    if (desc.data()->interlocks.has_value())
      m_status.interlocks->append(desc.data()->interlocks.value());
    m_status.interlocks->removeDuplicates();
  } else {
    m_status.interlocks->clear();
  }
  emit statusChanged(m_status);
}
