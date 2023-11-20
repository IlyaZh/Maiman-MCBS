#include "buttonwidget.h"

#include <QDebug>
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

ButtonWidget::ButtonWidget(const QString &text, const Button &button,
                           QSharedPointer<CommandConverter> converter,
                           QWidget *parent)
    : GuiWidgetBase(parent), m_converter(converter), m_description(button) {
  m_layout = new QHBoxLayout(this);
  m_layout->setMargin(0);
  m_layout->setSpacing(0);
  m_button = new QPushButton(text, parent);

  connect(m_button, &QPushButton::clicked, this, &ButtonWidget::buttonClicked);

  m_button->setMinimumSize(234, 36);
  m_button->setMaximumHeight(36);
  m_button->setMaximumWidth(234);
  m_button->setFont(QFont("Share Tech Mono", 18));
  m_button->setStyleSheet(buttonOff);
  m_button->setChecked(false);
  m_button->setVisible(true);
  m_layout->addWidget(m_button);
}

ButtonWidget::~ButtonWidget(){};

void ButtonWidget::setData(quint16 code, quint16 data) {
  if (code != m_converter->code()) return;
  m_converter->setValue(data);
  m_button->setStyleSheet(((data & m_description.mask) != 0) ? buttonOn
                                                             : buttonOff);
}

void ButtonWidget::buttonClicked() {
  emit setDataFromWidget(m_converter->code(),
                         (m_converter->valueInt() & m_description.mask)
                             ? m_description.offCommand
                             : m_description.onCommand);
}

QVector<quint16> ButtonWidget::Subscribe() {
  m_codes.append(m_converter->code());
  return m_codes;
}
