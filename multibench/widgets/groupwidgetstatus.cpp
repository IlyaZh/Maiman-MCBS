#include "groupwidgetstatus.h"

#include "ui_groupwidgetstatus.h"
#include "widgets/groupwidget.h"

const QString GroupWidgetStatus::startedStyleOn =
    "QLabel { \
        background: rgb(0,102,51); \
        border: 1px solid rgb(26,26,26); \
        border-radius: 3px; \
}";

const QString GroupWidgetStatus::startedStyleOff =
    "QLabel { \
        background: rgb(175,0,0); \
        border: 1px solid rgb(26,26,26); \
        border-radius: 3px; \
}";

GroupWidgetStatus::GroupWidgetStatus(QWidget *parent)
    : QWidget(parent), ui(new Ui::GroupWidgetStatus) {
  ui->setupUi(this);
}

GroupWidgetStatus::~GroupWidgetStatus() { delete ui; }

void GroupWidgetStatus::addData(DeviceStatusGroup &status) {
  QString format(
      R"(Status: <span style=color:'%1'>Errors and Warnings</span>)");
  QString toolTip(
      R"(<span style='background-color: #FFFFFF; color: #000000'>%1</span>)");
  bool hasErrors = !status.errors->isEmpty();
  bool hasInterlock = !status.interlocks->isEmpty();
  if (hasErrors and hasInterlock) {
    ui->statusLabel->setToolTip(toolTip.arg(status.errors->join(" ;") + " " +
                                            status.interlocks->join(" ;")));
    ui->statusLabel->setText(format.arg("#FF403A"));
  } else if (hasErrors and !hasInterlock) {
    ui->statusLabel->setToolTip(toolTip.arg(status.errors->join(" ;")));
    ui->statusLabel->setText(format.arg("#FF403A"));
  } else if (!hasErrors and hasInterlock) {
    ui->statusLabel->setToolTip(toolTip.arg(status.interlocks->join(" ;")));
    ui->statusLabel->setText(format.arg("#FFC803"));
  } else {
    ui->statusLabel->setToolTip(QString());
    ui->statusLabel->setText(format.arg("#FFFFFF"));
  }

  if (status.devStarted.has_value()) {
    for (auto map = status.devStarted->cbegin(),
              end = status.devStarted->cend();
         map != end; ++map) {
      if (!m_devs.contains(map.key())) {
        auto devLabel = new QLabel(map.key());
        devLabel->setFont(QFont("Poppins", 14));
        devLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        devLabel->setAlignment(Qt::AlignHCenter | Qt::AlignLeft);
        devLabel->setMaximumWidth(100);
        devLabel->setMinimumWidth(100);
        ui->labelsLayout->addWidget(devLabel);
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
                                     border: 1px solid rgb(26,26,26); \
                                     border-radius: 3px; \
                             }");
  } else {
    ui->linkLabel->setStyleSheet(
        "QLabel { \
                                 background: rgb(175,0,0); \
                                 border: 1px solid rgb(26,26,26); \
                                 border-radius: 3px; \
                         }");
  }
}

void GroupWidgetStatus::updateStyle() { this->update(); }
