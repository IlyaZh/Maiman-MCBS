#include "deviceholder.h"

DeviceHolder::DeviceHolder(
    const DeviceWidgetDesc& description,
    const QMap<quint16, QSharedPointer<CommandConverter>>& converters,
    QWidget* parent)
    : QWidget(parent),
      m_description(description),
      m_converters(converters),
      m_widgetLayout(new QVBoxLayout()),
      m_expandedWidget(new DeviceWidget(m_description, m_converters)),
      m_foldedWidget(new DeviceFoldedWidget()) {
  m_id = m_description.id;
  this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
  m_widgetLayout->setMargin(0);
  m_widgetLayout->setSpacing(0);
  m_widgetLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  m_widgetLayout->addWidget(m_expandedWidget);
  m_widgetLayout->addWidget(m_foldedWidget);
  m_foldedWidget->hide();
  this->setLayout(m_widgetLayout);
  connect(m_expandedWidget, &DeviceWidget::acceptDataFromWidget, this,
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
    m_expandedWidget->updateValue(event);
  } else if (std::holds_alternative<model::events::network::ChangeSystemStyle>(
                 event.data_)) {
    updateStyle();
  }
}

void DeviceHolder::setAddress(int addr) {
  m_address = addr;
  m_expandedWidget->setAddress(m_address);
  m_name = m_expandedWidget->getName();
}

int DeviceHolder::getAddress() const { return m_address; }

int DeviceHolder::getId() const { return m_id; }
QString DeviceHolder::getName() const { return m_name; }
QString DeviceHolder::getModel() const { return m_description.name; }

void DeviceHolder::updateStyle() { m_expandedWidget->updateStyle(); }

void DeviceHolder::setLink(bool link) { m_expandedWidget->setLink(link); }
void DeviceHolder::setConstraint(bool state) {
  m_expandedWidget->setConstraint(state);
}

void DeviceHolder::hideControlsButtonClicked() {
  m_expandedWidget->hide();
  m_foldedWidget->show();
  m_widgetLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
  this->adjustSize();
}
void DeviceHolder::showWidgetButtonClicked() {
  m_expandedWidget->show();
  m_foldedWidget->hide();
  m_widgetLayout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
  this->adjustSize();
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
