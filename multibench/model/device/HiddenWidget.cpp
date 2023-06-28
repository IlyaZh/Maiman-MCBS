#include "HiddenWidget.h"

#include "ui_HiddenWidget.h"

HiddenWidget::HiddenWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::HiddenWidget) {
  ui->setupUi(this);
  m_layout = new QVBoxLayout();
  m_layout->setSpacing(0);
  m_layout->setContentsMargins(0, 0, 0, 0);

  ui->widget->setLayout(m_layout);
}

HiddenWidget::~HiddenWidget() { delete ui; }

void HiddenWidget::addWidget(QWidget* newWidget) {
  m_widgets.append(newWidget);
  newWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
  m_layout->insertWidget(m_widgets.size(), newWidget);
  newWidget->adjustSize();
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
    widget->adjustSize();
    widget->hide();
    widget->show();
  }
}

bool HiddenWidget::isShown() const { return m_isVisible; }

void HiddenWidget::setShown(bool show) {
  m_isVisible = show;
  for (auto* widget : qAsConst(m_widgets)) widget->setVisible(m_isVisible);
  this->adjustSize();
}

bool HiddenWidget::isPinned() const { return m_isPinned; }

void HiddenWidget::setPinned(bool pin) { m_isPinned = pin; }
