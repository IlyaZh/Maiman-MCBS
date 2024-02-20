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

static const QFont standartFont = QFont("Share Tech Mono", 14);

GroupWidgetStatus::GroupWidgetStatus(QWidget *parent)
    : QWidget(parent), ui(new Ui::GroupWidgetStatus) {
  ui->setupUi(this);
}

GroupWidgetStatus::~GroupWidgetStatus() { delete ui; }

void GroupWidgetStatus::addData(DeviceStatusGroup &status) {
  if (status.errors.has_value()) {
    ui->warningsLabel->setText(status.errors->join(" ;"));
  }
  if (status.devStarted.has_value()) {
    auto map = status.devStarted->keys();
    for (auto dev : map) {
      if (!m_devs.contains(dev)) {
        auto devLabel = new QLabel(dev);
        devLabel->setFont(standartFont);
        devLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        devLabel->setMaximumWidth(100);
        ui->labelsLayout->addWidget(devLabel);
        m_devs.insert(dev, devLabel);
      }
      if (status.devStarted->value(dev)) {
        m_devs.value(dev)->setText(dev + QString(": ON"));
      } else {
        m_devs.value(dev)->setText(dev + QString(": OFF"));
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
