#include "HiddenWidget.h"
#include "ui_HiddenWidget.h"

HiddenWidget::HiddenWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HiddenWidget)
{
    ui->setupUi(this);
    m_layout = new QVBoxLayout();
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0,0,0,0);

    this->setLayout(m_layout);
}

HiddenWidget::~HiddenWidget()
{
    delete ui;
}

void HiddenWidget::addWidget(QWidget* widget) {
    m_widgets.append(widget);
    widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    m_layout->insertWidget(m_widgets.size(), widget);
    this->adjustSize();
}

QVector<QWidget*> HiddenWidget::widgets() {
    return m_widgets;
}

bool HiddenWidget::isVisible() {
    return m_isVisible;
}

void HiddenWidget::setVisible(bool show) {
    m_isVisible = show;
    for(auto widget : m_widgets)
        widget->setVisible(m_isVisible);
    this->adjustSize();
}

bool HiddenWidget::isPinned() {
    return m_isPinned;
}

void HiddenWidget::setPinned(bool pin) {
    m_isPinned = pin;
}
