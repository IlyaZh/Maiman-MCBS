#include "devicefoldedwidget.h"

#include "ui_devicefoldedwidget.h"
#include "widgets/buttonwidget.h"
#include "widgets/foldedcontrolwidget.h"

DeviceFoldedWidget::DeviceFoldedWidget(
    const DeviceWidgetDesc& description,
    const QMap<quint16, QSharedPointer<CommandConverter>>& converters,
    QWidget* parent)
    : QWidget(parent),
      ui(new Ui::DeviceFoldedWidget),
      m_buttons(description.buttons),
      m_converters(converters),
      m_widgetLayout(new QHBoxLayout()) {
  ui->setupUi(this);
  setObjectName("DeviceFoldedWidget");
  setAttribute(Qt::WA_StyledBackground, true);
  ui->modelLabel->setText(description.name);
  m_warning = new WarningWidget();
  ui->buttonsLayout->addWidget(m_warning);
  ui->buttonsLayout->setAlignment(m_warning, Qt::AlignVCenter | Qt::AlignRight);
  ui->widgetsLayout->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  ui->widgetsLayout->setContentsMargins(0, 0, 0, 0);
  for (const auto& control : description.controls) {
    auto valueConverter = m_converters.value(control.value, nullptr);
    auto realConverter = m_converters.value(control.real, nullptr);

    if (valueConverter != nullptr) {
      // Обработка изменяемых параметров
      auto widget = new FoldedControlWidget(control.name, valueConverter,
                                            realConverter, this);
      widget->setContentsMargins(10, 0, 10, 0);
      m_widgetLayout->addWidget(widget);
      m_folded.insert(widget);
      m_widgetsTable.insert(widget);
    }
  }
  for (const auto& button : qAsConst(description.buttons)) {
    QPointer<ButtonWidget> pButton;
    if (button.name.compare("laser", Qt::CaseInsensitive) == 0 &&
        m_converters.contains(button.code)) {
      auto converter = m_converters.value(button.code);
      m_laserButton = new ButtonWidget("Laser", button, converter, this);
      pButton = m_laserButton;
      m_laserButton->setSize(true);
      m_widgetsTable.insert(m_laserButton);
    } else if (button.name.compare("TEC", Qt::CaseInsensitive) == 0) {
      auto converter = m_converters.value(button.code);
      m_tecButton = new ButtonWidget("TEC", button, converter, this);
      pButton = m_tecButton;
      m_tecButton->setSize(true);
      m_widgetsTable.insert(m_tecButton);
    }

    if (pButton) {
      ui->buttonsLayout->setContentsMargins(0, 0, 0, 0);
      ui->buttonsLayout->addWidget(pButton);
      ui->buttonsLayout->setAlignment(pButton,
                                      Qt::AlignHCenter | Qt::AlignRight);
    }
  }
  m_widgetLayout->setContentsMargins(0, 0, 0, 0);
  m_widgetLayout->setSpacing(0);
  // m_widgetLayout->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
  ui->widgetsLayout->addLayout(m_widgetLayout);
  for (auto widget : qAsConst(m_widgetsTable)) {
    connect(widget, &GuiWidgetBase::setDataFromWidget, this,
            &DeviceFoldedWidget::acceptDataFromWidget);
  }
  connect(ui->showButton, &QPushButton::clicked, this,
          &DeviceFoldedWidget::showWidget);
  DeviceFoldedWidget::updateStyle();
}

DeviceFoldedWidget::~DeviceFoldedWidget() { delete ui; }

void DeviceFoldedWidget::updateValue(const model::Event& event) {
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
  ui->showButton->setStyleSheet(StyleStorage::Device::hideButton());
  this->setStyleSheet(StyleStorage::FoldedDevice::widget());
  this->update();
}

void DeviceFoldedWidget::setStatus(DeviceStatusGroup& status) {
  m_warning->addData(status);
}

void DeviceFoldedWidget::setPinnedWidgets(QMap<QString, bool>& widgets) {
  for (auto widget : qAsConst(m_folded)) {
    if (widgets.contains(widget->getName())) {
      widget->setVisible(widgets.value(widget->getName()));
    }
  }
}
