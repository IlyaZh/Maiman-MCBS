#include "warningwidget.h"

#include <QToolTip>

using namespace StyleStorage::Group;

WarningWidget::WarningWidget(QWidget *parent) : QWidget(parent) {
  setObjectName("GroupWidgetStatus");
  setAttribute(Qt::WA_StyledBackground, true);
  m_iconHolder = new QPushButton(this);
  m_warning = new QLabel(this);
  m_layout = new QHBoxLayout(this);
  setMaximumSize(90, 21);
  setMinimumSize(90, 21);
  setStyleSheet(R"(background: transparent;)");
  QFont font12("Poppins", 12);
  font12.setLetterSpacing(QFont::PercentageSpacing, 105);
  font12.setPixelSize(18);
  m_warning->setFont(font12);
  m_iconPlug.addFile(QString::fromUtf8(":/resources/images/GroupPlug.png"),
                     QSize(16, 16), QIcon::Normal, QIcon::On);
  m_iconWarning.addFile(
      QString::fromUtf8(":/resources/images/GroupWarning.png"), QSize(16, 16),
      QIcon::Normal, QIcon::On);
  m_iconError.addFile(QString::fromUtf8(":/resources/images/GroupError.png"),
                      QSize(16, 16), QIcon::Normal, QIcon::On);
  m_iconHolder->setIcon(m_iconPlug);
  m_iconHolder->setIconSize(QSize(16, 16));
  m_iconHolder->setFixedSize(16, 16);
  m_iconHolder->setStyleSheet("border: 0px;");

  m_layout->addWidget(m_warning);
  m_layout->addWidget(m_iconHolder);
  m_layout->setMargin(0);
  m_layout->setSpacing(0);
  m_layout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  m_layout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
  this->setLayout(m_layout);
  WarningWidget::updateStyle();
  m_customToolTip = new CustomTooltip(this);
  m_customToolTip->setAlignment(Qt::AlignLeft);
  m_warning->installEventFilter(this);
  m_iconHolder->installEventFilter(this);
  m_toolTip = "";
}

WarningWidget::WarningWidget(bool inv, QWidget *parent)
    : QWidget(parent), m_isInv(inv) {
  setObjectName("GroupWidgetStatus");
  setAttribute(Qt::WA_StyledBackground, true);
  m_iconHolder = new QPushButton(this);
  m_warning = new QLabel(this);
  m_layout = new QHBoxLayout(this);
  setMaximumSize(90, 32);
  setMinimumSize(90, 32);
  setStyleSheet(R"(background: transparent; margin-top:3px;)");
  QFont font12("Poppins", 12);
  font12.setLetterSpacing(QFont::PercentageSpacing, 105);
  font12.setPixelSize(18);
  m_warning->setFont(font12);
  m_warning->setAlignment(Qt::AlignLeft);
  m_iconPlug.addFile(QString::fromUtf8(":/resources/images/GroupPlug.png"),
                     QSize(16, 16), QIcon::Normal, QIcon::On);
  m_iconWarning.addFile(
      QString::fromUtf8(":/resources/images/GroupWarning.png"), QSize(16, 16),
      QIcon::Normal, QIcon::On);
  m_iconError.addFile(QString::fromUtf8(":/resources/images/GroupError.png"),
                      QSize(16, 16), QIcon::Normal, QIcon::On);
  m_iconHolder->setIcon(m_iconPlug);
  m_iconHolder->setIconSize(QSize(16, 32));
  m_iconHolder->setFixedSize(16, 32);
  m_iconHolder->setStyleSheet("border: 0px;");

  m_layout->addWidget(m_iconHolder);
  m_layout->addWidget(m_warning);
  m_layout->addSpacerItem(new QSpacerItem(0, 21, QSizePolicy::Expanding));
  m_layout->setMargin(0);
  m_layout->setSpacing(0);
  m_layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  m_layout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
  this->setLayout(m_layout);
  WarningWidget::updateStyle();
  m_customToolTip = new CustomTooltip(this);
  m_customToolTip->setAlignment(Qt::AlignLeft);
  m_warning->installEventFilter(this);
  m_iconHolder->installEventFilter(this);
  m_toolTip = "";
}

bool WarningWidget::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::Enter) {
    QPoint pos = static_cast<QWidget *>(obj)->mapToGlobal(QPoint(10, 20));
    if (obj == m_warning) {
      m_customToolTip->showTooltip(m_toolTip, pos);
    } else if (obj == m_iconHolder) {
      m_customToolTip->showTooltip(m_toolTip, pos);
    }
    return true;
  } else if (event->type() == QEvent::Leave) {
    m_customToolTip->hideTooltip();
    return true;
  }
  return QWidget::eventFilter(obj, event);
}
void WarningWidget::updateStyle() { this->update(); }

void WarningWidget::addData(DeviceStatusGroup &status) {
  QString toolTip(R"(%1)");

  bool hasErrors = !status.errors->isEmpty();
  bool hasInterlock = !status.interlocks->isEmpty();
  if (hasErrors and hasInterlock) {
    m_toolTip = toolTip.arg(QString(R"(<font color=#FF403A>%1</font>)")
                                .arg(status.errors->join("<br>")) +
                            "<br>" +
                            QString(R"(<font color=#FFC803>%1</font>)")
                                .arg(status.interlocks->join("<br>")));
    m_warning->setVisible(true);
    m_warning->setStyleSheet(Status::warningRed());
    m_warning->setText("Error");
    m_layout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_iconHolder->setIcon(m_iconError);

  } else if (hasErrors and !hasInterlock) {
    m_toolTip = toolTip.arg(QString(R"(<font color=#FF403A>%1</font>)")
                                .arg(status.errors->join("<br>")));
    m_warning->setVisible(true);
    m_warning->setStyleSheet(Status::warningRed());
    m_warning->setText("Error");
    m_layout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_iconHolder->setIcon(m_iconError);

  } else if (!hasErrors and hasInterlock) {
    m_toolTip = toolTip.arg(QString(R"(<font color=#FFC803>%1</font>)")
                                .arg(status.interlocks->join("<br>")));
    m_warning->setVisible(true);
    m_warning->setStyleSheet(Status::warningYellow());
    m_warning->setText("Warning");
    m_layout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_iconHolder->setIcon(m_iconWarning);

  } else {
    m_toolTip = "";
    m_warning->setVisible(false);
    m_warning->setStyleSheet(Status::warningGray());
    m_warning->setText("Warning");
    m_layout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_iconHolder->setIcon(m_iconPlug);
  }
}

void WarningWidget::addDevicesData(
    QMap<quint8, QSharedPointer<DeviceStatusGroup>> &status) {
  QStringList devs;
  m_toolTip = "";
  bool allErrors = false;
  bool allInterlock = false;
  for (auto it = status.begin(); it != status.end(); ++it) {
    QString dev(R"(ID: %1 - %2)");
    bool hasErrors = !it.value()->errors->isEmpty();
    bool hasInterlock = !it.value()->interlocks->isEmpty();
    allErrors |= hasErrors;
    allInterlock |= hasInterlock;
    if (hasErrors and hasInterlock) {
      devs.append(
          dev.arg(it.key()).arg(QString(R"(<font color=#FF403A>%1</font>)")
                                    .arg(it.value()->errors->join(" ")) +
                                " " +
                                QString(R"(<font color=#FFC803>%1</font>)")
                                    .arg(it.value()->interlocks->join(" "))));

    } else if (hasErrors and !hasInterlock) {
      devs.append(
          dev.arg(it.key()).arg(QString(R"(<font color=#FF403A>%1</font>)")
                                    .arg(it.value()->errors->join(" "))));

    } else if (!hasErrors and hasInterlock) {
      devs.append(
          dev.arg(it.key()).arg(QString(R"(<font color=#FFC803>%1</font>)")
                                    .arg(it.value()->interlocks->join(" "))));

    } else {
      dev = "";
    }
  }
  if (allErrors) {
    m_warning->setVisible(true);
    m_warning->setStyleSheet(Status::warningRedInv());
    m_warning->setText("Error");
    m_layout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_iconHolder->setIcon(m_iconError);
  } else if (!allErrors and allInterlock) {
    m_warning->setVisible(true);
    m_warning->setStyleSheet(Status::warningYellowInv());
    m_warning->setText("Warning");
    m_layout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_iconHolder->setIcon(m_iconWarning);
  } else if (!allErrors and !allInterlock) {
    m_warning->setVisible(false);
    m_warning->setStyleSheet(Status::warningGrayInv());
    m_warning->setText("Warning");
    m_layout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_iconHolder->setIcon(m_iconPlug);
  }
  m_toolTip = devs.join("<br>");
}
