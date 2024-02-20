#include "inlineedit.h"

#include <QMouseEvent>

const QString readOnly =
    "QLineEdit{background-color: rgb(51, 51, 51);\
    color: rgb(255,255,255);\
    border-color: rgb(51, 51, 51);\
    border: 1px;\
    font: 20pt Share Tech Mono;}";

const QString editOnly =
    "QLineEdit{background-color: rgb(26, 26, 26);\
    color: rgb(255,255,255);\
    border-color: rgb(26, 26, 26);\
    border: 1px;\
    border-radius: 6px; \
    font: 20pt Share Tech Mono;}";

enum {
  maxLengthString = 13,
  lenghtNonEditableString = 4,
  groupMaxLengthString = 29
};

InLineEdit::InLineEdit(int addr, bool isDevice)
    : m_name(""),
      m_address(addr),
      m_icon(new QPushButton()),
      m_layout(new QHBoxLayout),
      m_isDevice(isDevice) {
  m_layout = new QHBoxLayout();
  m_icon = new QPushButton();
  m_layout->setContentsMargins(0, 0, 0, 0);
  this->setContentsMargins(0, 0, 0, 0);

  m_layout->setSpacing(0);
  m_icon->setIconSize(QSize(10, 10));
  m_icon->setStyleSheet(
      QString::fromUtf8("border: 2px solid rgb(26,26,26);\n"
                        "border-radius: 3px;\n"
                        "background: rgb(17,17,17);\n"
                        "color: rgb(255,255,255);\n"
                        "padding: 0px;\n"
                        "margin-bottom: 9px;"));
  QIcon icon1;
  icon1.addFile(QString::fromUtf8(":/resources/images/pencil.png"), QSize(),
                QIcon::Normal, QIcon::Off);
  m_icon->setIcon(icon1);
  m_icon->setIconSize(QSize(8, 12));
  m_icon->setMinimumSize(10, 10);
  m_icon->setCheckable(false);
  m_layout->addWidget(m_icon, 0, Qt::AlignRight);
  this->setLayout(m_layout);

  setText(m_name);
  setReadOnly(true);
  setStyleSheet(readOnly);
  setAlignment(Qt::AlignLeft);
  if (m_isDevice)
    setMaximumWidth(200);
  else
    setMaximumWidth(450);
  setMaximumHeight(30);
  setTextMargins(0, 0, 0, 0);
  connect(this, &InLineEdit::editingFinished, this,
          &InLineEdit::finishedChanges);
  connect(this, &InLineEdit::textEdited, this, &InLineEdit::checkTextLenght);
  connect(m_icon, &QPushButton::clicked, this, [this]() {
    m_icon->hide();
    setReadOnly(false);
    setStyleSheet(editOnly);
    QLineEdit::setText(m_name);
    setFocus(Qt::FocusReason::MouseFocusReason);
  });
}

void InLineEdit::mouseDoubleClickEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    setReadOnly(false);
    setStyleSheet(editOnly);
    m_icon->hide();
    QLineEdit::setText(m_name);
  }
}

void InLineEdit::finishedChanges() {
  setReadOnly(true);
  setStyleSheet(readOnly);
  if (!QLineEdit::text().contains("ID:")) {
    m_name = QLineEdit::text();
    emit nameEdited(m_name, m_address);
  }
  setText(m_name);
  clearFocus();
}

void InLineEdit::setText(const QString &text) {
  m_name = text;
  if (m_name == "") {
    m_icon->show();
    if (m_isDevice)
      QLineEdit::setText(QString("ID:%1").arg(m_address));
    else
      QLineEdit::setText(QString("Group %1").arg(m_address));
  } else {
    m_icon->hide();
    if (m_isDevice)
      QLineEdit::setText(QString("%1 ID:%2").arg(text).arg(m_address));
    else
      QLineEdit::setText(text);
  }
}

QString InLineEdit::text() { return QLineEdit::text(); }

void InLineEdit::checkTextLenght() {
  if (m_isDevice) {
    if (!QLineEdit::text().contains("ID:")) {
      m_name = QLineEdit::text();
      if (m_name.size() + lenghtNonEditableString +
              QString::number(m_address).size() >
          maxLengthString) {
        m_name = m_name.remove(maxLengthString - lenghtNonEditableString -
                                   QString::number(m_address).size(),
                               m_name.size());
        QLineEdit::setText(m_name);
      }
    }
  } else {
    m_name = QLineEdit::text();
    if (m_name.size() > groupMaxLengthString) {
      m_name = m_name.remove(groupMaxLengthString, m_name.size());
      QLineEdit::setText(m_name);
    }
  }
}
