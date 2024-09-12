#include "inlineedit.h"

#include <QMouseEvent>

const QString readOnly =
    "QLineEdit{background-color: #282828;\
    color: rgb(255,255,255);\
    border-color: #6B6B6B;\
    border: 1px;\
}";

const QString editOnlyConnected =
    "QLineEdit{background-color: #222222;\
    color: #39CA76;\
    border-color: #6B6B6B;\
    border: 1px;\
    border-radius: 6px; \
}";

const QString editOnlyDisconnected =
    "QLineEdit{background-color: #222222;\
    color: #FF403A;\
    border-color: #6B6B6B;\
    border: 1px;\
    border-radius: 6px; \
}";

const QString devConnected =
    "QLineEdit{background-color: #282828;\
    color: #39CA76;\
    border-color: #6B6B6B;\
    border: 1px;\
}";

const QString devDisconnected =
    "QLineEdit{background-color: #282828;\
    color: #FF403A;\
    border-color: #6B6B6B;\
    border: 1px;\
}";

enum {
  maxLengthString = 13,
  lenghtNonEditableString = 5,
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
  m_layout->setContentsMargins(0, 0, 0, 2);
  this->setContentsMargins(0, 0, 0, 0);

  m_layout->setSpacing(0);
  //  m_icon->setIconSize(QSize(12, 12));
  m_icon->setStyleSheet(
      QString::fromUtf8("border: 0px solid rgb(26,26,26);\n"
                        "border-radius: 3px;\n"
                        "background: #1A1A1A;\n"
                        "color: #8E8E8E;\n"
                        "padding: 0px;\n"
                        "margin-bottom: 0px;"));
  QIcon icon1;
  QPixmap pixmap(":/resources/images/NewPencil.png");
  pixmap = pixmap.scaled(14, 14, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  //  icon1.addFile(QString::fromUtf8(":/resources/images/NewPencil.png"),
  //                QSize(12, 12), QIcon::Normal, QIcon::Off);
  icon1.addPixmap(pixmap, QIcon::Normal, QIcon::Off);
  m_icon->setIcon(icon1);
  m_icon->setIconSize(QSize(14, 14));
  m_icon->setMinimumSize(16, 16);
  m_icon->setMaximumSize(16, 16);
  m_icon->setCheckable(false);
  m_layout->addWidget(m_icon, 0, Qt::AlignRight);
  this->setLayout(m_layout);
  QFont font16("Poppins Medium", 16);
  font16.setLetterSpacing(QFont::PercentageSpacing, 105);
  setFont(font16);
  setText(m_name);
  setReadOnly(true);
  setStyleSheet(devConnected);
  setAlignment(Qt::AlignLeft);
  setContentsMargins(0, 2, 0, 0);
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
    setStyleSheet(m_isConnected ? editOnlyConnected : editOnlyDisconnected);
    QLineEdit::setText(m_name);
    setFocus(Qt::FocusReason::MouseFocusReason);
  });
}

void InLineEdit::mouseDoubleClickEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    setReadOnly(false);
    setStyleSheet(m_isConnected ? editOnlyConnected : editOnlyDisconnected);
    m_icon->hide();
    QLineEdit::setText(m_name);
  }
}

void InLineEdit::finishedChanges() {
  setReadOnly(true);
  setStyleSheet(m_isConnected ? devConnected : devDisconnected);
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
      QLineEdit::setText(QString("ID: %1").arg(m_address));
    else
      QLineEdit::setText(QString("Group %1").arg(m_address));
  } else {
    m_icon->hide();
    if (m_isDevice)
      QLineEdit::setText(QString("%1 ID: %2").arg(text).arg(m_address));
    else
      QLineEdit::setText(text);
  }
}

QString InLineEdit::text() { return QLineEdit::text(); }

void InLineEdit::checkTextLenght() {
  if (m_isDevice) {
    if (!QLineEdit::text().contains("ID: ")) {
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

void InLineEdit::setLink(bool link) {
  if (link != m_isConnected) {
    m_isConnected = link;
    setStyleSheet(m_isConnected ? devConnected : devDisconnected);
  }
}

void InLineEdit::setAddress(int addr) {
  m_address = addr;
  setText(m_name);
}
