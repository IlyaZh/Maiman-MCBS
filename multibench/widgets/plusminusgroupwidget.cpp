#include "plusminusgroupwidget.h"
#include "plusminuswidget.h"

PlusMinusGroupWidget::PlusMinusGroupWidget(PlusMinusWidget* min, PlusMinusWidget* max, QWidget *parent)
    : QWidget{parent},
      m_minWidget{min},
      m_maxWidget{max},
      m_layout{new QVBoxLayout(this)}
{
    m_layout->addWidget(m_maxWidget);
    m_layout->addWidget(m_minWidget);
    m_layout->setContentsMargins(0,0,0,0);
    connect(m_minWidget, &PlusMinusWidget::editFinished, this, [this](){
        m_maxWidget->setMin(m_minWidget->value());
    });

    connect(m_maxWidget, &PlusMinusWidget::editFinished, this, [this](){
        m_minWidget->setMax(m_maxWidget->value());
    });
}
