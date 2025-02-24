#include "HiddenWidget.h"

#include "ui_HiddenWidget.h"

HiddenWidget::HiddenWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::HiddenWidget) {
  ui->setupUi(this);
  m_layout = new QVBoxLayout();
  //  m_layout->setSpacing(0);

  ui->widget->setLayout(m_layout);
  HiddenWidget::updateStyle();
}

HiddenWidget::~HiddenWidget() { delete ui; }

void HiddenWidget::addWidget(QWidget* newWidget) {
  m_widgets.append(newWidget);
  newWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
  m_layout->insertWidget(m_widgets.size(), newWidget);
  //  newWidget->adjustSize();
  this->adjustSize();

  int maxWidth = 0;
  for (const auto& widget : qAsConst(m_widgets)) {
    if (widget->size().width() > maxWidth) maxWidth = widget->size().width();
  }
  auto newSize = QSize(maxWidth, 0);

  for (const auto& widget : qAsConst(m_widgets)) {
    auto size = widget->size();
    newSize.setHeight(size.height());
    widget->setMinimumWidth(maxWidth);
    widget->resize(newSize);
    //    widget->adjustSize();
    widget->hide();
    widget->show();
  }
}

bool HiddenWidget::isShown() const { return m_isVisible; }

void HiddenWidget::setShown(bool show) {
  m_isVisible = show;
  for (auto* widget : qAsConst(m_widgets)) widget->setVisible(m_isVisible);
  if (m_isVisible) {
    m_layout->setContentsMargins(m_margin);
    m_layout->update();
  } else {
    //    m_margin = m_layout->contentsMargins();
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->update();
  }
  this->adjustSize();
}

bool HiddenWidget::isPinned() const { return m_isPinned; }

void HiddenWidget::setPinned(bool pin) { m_isPinned = pin; }

void HiddenWidget::setMargins(int top, int left, int bottom, int right) {
  m_layout->setContentsMargins(top, left, bottom, right);
  m_margin = m_layout->contentsMargins();
  m_layout->update();
}

void HiddenWidget::updateStyle() {
  this->setStyleSheet(StyleStorage::Device::hiddenWidget());
  this->update();
}
