#include "devicefoldedwidget.h"

#include "ui_devicefoldedwidget.h"

DeviceFoldedWidget::DeviceFoldedWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::DeviceFoldedWidget) {
  ui->setupUi(this);
  m_warning = new WarningWidget();
  ui->buttonsLayout->addWidget(m_warning);
  connect(ui->showButton, &QPushButton::clicked, this,
          &DeviceFoldedWidget::showWidget);
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

void DeviceFoldedWidget::updateStyle() {}

void DeviceFoldedWidget::setStatus(DeviceStatusGroup& status) {
  m_warning->addData(status);
}
