#include "groupwidgetstatus.h"

#include "ui_groupwidgetstatus.h"
#include "widgets/groupwidget.h"

GroupWidgetStatus::GroupWidgetStatus(QWidget *parent)
    : QWidget(parent), ui(new Ui::GroupWidgetStatus) {
  ui->setupUi(this);
  m_iconHolder = new QPushButton();
  QFont font12("Poppins", 12);
  font12.setLetterSpacing(QFont::PercentageSpacing, 105);
  font12.setPixelSize(16);
  ui->horizontalLayout->setAlignment(Qt::AlignLeft);
  m_iconWarning.addFile(
      QString::fromUtf8(":/resources/images/GroupWarning.png"), QSize(16, 16),
      QIcon::Normal, QIcon::On);
  m_iconError.addFile(QString::fromUtf8(":/resources/images/GroupError.png"),
                      QSize(16, 16), QIcon::Normal, QIcon::On);
  m_iconHolder->setIcon(m_iconWarning);
  m_iconHolder->setIconSize(QSize(16, 16));
  m_iconHolder->setMinimumSize(16, 16);
  m_iconHolder->setMaximumSize(16, 16);
  m_iconHolder->setStyleSheet("border: 0px;");
  ui->horizontalLayout->addWidget(m_iconHolder, 1,
                                  Qt::AlignLeft | Qt::AlignVCenter);
  m_iconHolder->hide();

  ui->statusLabel->setFont(font12);
  ui->modelLabel->setFont(font12);
  ui->nameLabel->setFont(font12);
}

GroupWidgetStatus::~GroupWidgetStatus() { delete ui; }

void GroupWidgetStatus::addData(DeviceStatusGroup &status) {
  QString format(R"(<span style=color:'%1'>Warning</span>)");
  QString toolTip(
      R"(<span style='background-color: #FFFFFF; color: #000000'>%1</span>)");
  bool hasErrors = !status.errors->isEmpty();
  bool hasInterlock = !status.interlocks->isEmpty();
  if (hasErrors and hasInterlock) {
    ui->statusLabel->setToolTip(toolTip.arg(status.errors->join("\n") + "\n" +
                                            status.interlocks->join("\n")));
    ui->statusLabel->setText(format.arg("#FF403A"));
    m_iconHolder->setToolTip(toolTip.arg(status.errors->join("\n") + "\n" +
                                         status.interlocks->join("\n")));
    m_iconHolder->setIcon(m_iconError);
    m_iconHolder->show();
  } else if (hasErrors and !hasInterlock) {
    ui->statusLabel->setToolTip(toolTip.arg(status.errors->join("\n")));
    ui->statusLabel->setText(format.arg("#FF403A"));
    m_iconHolder->setToolTip(toolTip.arg(status.errors->join("\n")));
    m_iconHolder->setIcon(m_iconError);
    m_iconHolder->show();
  } else if (!hasErrors and hasInterlock) {
    ui->statusLabel->setToolTip(toolTip.arg(status.interlocks->join("\n")));
    ui->statusLabel->setText(format.arg("#FFC803"));
    m_iconHolder->setToolTip(toolTip.arg(status.interlocks->join("\n")));
    m_iconHolder->setIcon(m_iconWarning);
    m_iconHolder->show();
  } else {
    ui->statusLabel->setToolTip(QString());
    ui->statusLabel->setText(format.arg("#FFFFFF"));
    m_iconHolder->setToolTip(QString());
    m_iconHolder->hide();
  }

  if (status.devStarted.has_value()) {
    for (auto map = status.devStarted->cbegin(),
              end = status.devStarted->cend();
         map != end; ++map) {
      if (!m_devs.contains(map.key())) {
        auto devLabel = new QLabel(map.key());
        devLabel->setFont(QFont("Poppins", 14));
        devLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        devLabel->setAlignment(Qt::AlignHCenter | Qt::AlignLeft);
        devLabel->setMinimumSize(QSize(80, 24));
        devLabel->setMaximumSize(QSize(80, 24));
        //        ui->horizontalLayout->addWidget(devLabel);
        m_devs.insert(map.key(), devLabel);
      }
      if (map.value()) {
        m_devs.value(map.key())->setText(map.key() + QString(": ON"));
      } else {
        m_devs.value(map.key())->setText(map.key() + QString(": OFF"));
      }
    }
  }
}

void GroupWidgetStatus::setModel(const QString &name) {
  ui->modelLabel->setText(name);
}

void GroupWidgetStatus::setName(const QString &id) {
  ui->nameLabel->setText(id);
}

void GroupWidgetStatus::setLink(bool status) {
  if (status) {
    ui->linkLabel->setStyleSheet(
        "QLabel { \
                                     background: rgb(0,102,51); \
                                     border: 0px solid rgb(26,26,26); \
                                     border-radius: 3px; \
                             }");
  } else {
    ui->linkLabel->setStyleSheet(
        "QLabel { \
                                 background: rgb(175,0,0); \
                                 border: 0px solid rgb(26,26,26); \
                                 border-radius: 3px; \
                         }");
  }
}

void GroupWidgetStatus::updateStyle() { this->update(); }
