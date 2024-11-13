#include "inlinecontrol.h"

const QString readOnly =
    "QLineEdit{background-color: #222222;\
    border-radius: 12px;\
    border: 1px solid #6B6B6B;\
    color: rgba(255, 255, 255, 0.9);\
    text-align: center;\
}";

const QString editValue =
    "QLineEdit{background-color: #6B6B6B;\
    border-radius: 12px;\
    border: 1px solid #6B6B6B;\
    color: rgba(255, 255, 255, 0.9);\
    text-align: center;\
}";

const QString errorValue =
    "QLineEdit{background-color: #FF403A;\
    border-radius: 12px;\
    border: 1px solid #6B6B6B;\
    color: rgba(255, 255, 255, 0.9);\
    text-align: center;\
}";

InLineControl::InLineControl()
    : m_icon(new QPushButton()), m_layout(new QHBoxLayout) {
  m_layout = new QHBoxLayout();
  m_icon = new QPushButton();
  m_layout->setContentsMargins(0, 0, 3, 0);
  m_layout->setSpacing(0);
  //  m_icon->setIconSize(QSize(12, 12));
  m_icon->setStyleSheet(button());
  m_darkButton.addFile(
      QString::fromUtf8(":/resources/images/DarkSetButton.png"), QSize(38, 19),
      QIcon::Normal, QIcon::Off);
  m_lightButton.addFile(
      QString::fromUtf8(":/resources/images/LightSetButton.png"), QSize(38, 19),
      QIcon::Normal, QIcon::Off);
  m_icon->setIcon((AppSettings::getDarkAppStyle()) ? m_darkButton
                                                   : m_lightButton);
  m_icon->setIconSize(QSize(38, 19));
  m_icon->setMinimumSize(38, 19);
  m_icon->setMaximumSize(38, 19);
  m_icon->setCheckable(false);
  m_layout->addWidget(m_icon, 0, Qt::AlignRight | Qt::AlignVCenter);
  m_icon->hide();
  this->setLayout(m_layout);
  QFont font10("Poppins", 10);
  font10.setLetterSpacing(QFont::PercentageSpacing, 105);
  font10.setPixelSize(14);
  setFont(font10);
  setText(m_value);
  setReadOnly(true);
  setAlignment(Qt::AlignCenter);
  setMinimumSize(168, 25);
  setMaximumSize(168, 25);
  setMaxLength(6);
  setFocusPolicy(Qt::FocusPolicy::StrongFocus);
  setStyleSheet(read());
  connect(this, &InLineControl::editingFinished, this,
          &InLineControl::finishedChanges);
  //  connect(m_icon, &QPushButton::clicked, this, [this]() {
  //    finishedChanges();
  //    // setFocus(Qt::FocusReason::MouseFocusReason);
  //  });
  connect(m_icon, &QPushButton::clicked, this, &InLineControl::finishedChanges);
}

void InLineControl::mouseDoubleClickEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    setReadOnly(false);
    setActiveStyle();
    m_icon->show();
    QLineEdit::setText(m_value);
  }
}

void InLineControl::finishedChanges() {
  emit QLineEdit::returnPressed();
  setReadOnly(true);
  setText(m_value);
  m_icon->hide();
  //  setInactiveStyle();
  clearFocus();
}

void InLineControl::setText(const QString &text) {
  m_value = text;
  QLineEdit::setText(text);
}

QString InLineControl::text() { return QLineEdit::text(); }

void InLineControl::setActiveStyle() { setStyleSheet(edit()); }
void InLineControl::setInactiveStyle() { setStyleSheet(read()); }
void InLineControl::setErrorStyle() { setStyleSheet(error()); }

void InLineControl::updateStyle() {
  setStyleSheet(read());
  m_icon->setIcon((AppSettings::getDarkAppStyle()) ? m_darkButton
                                                   : m_lightButton);
  m_icon->setIconSize(QSize(38, 19));
  m_icon->setStyleSheet(button());
  m_icon->update();
  setInactiveStyle();
  this->update();
}
