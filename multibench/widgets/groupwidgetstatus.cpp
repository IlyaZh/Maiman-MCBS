#include "groupwidgetstatus.h"

#include "ui_groupwidgetstatus.h"
#include "widgets/groupwidget.h"

using namespace StyleStorage::Group;

GroupWidgetStatus::GroupWidgetStatus(QWidget *parent)
    : QWidget(parent), ui(new Ui::GroupWidgetStatus) {
  ui->setupUi(this);
  setObjectName("GroupWidgetStatus");
  setAttribute(Qt::WA_StyledBackground, true);
  ui->linkLabel->setAttribute(Qt::WA_StyledBackground, true);
  m_iconHolder = new QPushButton();
  QFont font12("Poppins", 14);
  font12.setLetterSpacing(QFont::PercentageSpacing, 105);
  font12.setPixelSize(18);
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
  ui->statusLayout->addWidget(m_iconHolder, 1,
                              Qt::AlignLeft | Qt::AlignVCenter);

  ui->horizontalLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  ui->statusLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  ui->statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  ui->statusLabel->setFont(font12);
  ui->modelLabel->setFont(font12);
  ui->nameLabel->setFont(font12);

  laserStarted = new QWidget();
  laserStarted->setFixedSize(112, 24);
  laserLayout = new QHBoxLayout(laserStarted);
  laserLayout->setContentsMargins(0, 0, 0, 0);  // Убираем отступы
  laserLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  laserLayout->addWidget(laserStarted);
  ui->horizontalLayout->addWidget(laserStarted);

  tecStarted = new QWidget();
  tecStarted->setFixedSize(112, 24);
  tecLayout = new QHBoxLayout(tecStarted);
  tecLayout->setContentsMargins(0, 0, 0, 0);  // Убираем отступы
  tecLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  tecLayout->addSpacerItem(
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));
  ui->horizontalLayout->addWidget(tecStarted);

  GroupWidgetStatus::updateStyle();

  ui->statusLabel->installEventFilter(this);
  m_iconHolder->installEventFilter(this);
  m_toolTip =
      R"(<span style='background-color: #FFFFFF; color: #000000'>%1</span>)";
}

GroupWidgetStatus::~GroupWidgetStatus() { delete ui; }

void GroupWidgetStatus::addData(DeviceStatusGroup &status) {
  QString toolTip(R"(<span style='white-space:pre'>%1</span>)");
  qDebug() << "GROUP" << status.errors.value() << status.interlocks.value();
  if (status.errors.has_value() or status.interlocks.has_value()) {
    bool hasErrors = !status.errors->isEmpty();
    bool hasInterlock = !status.interlocks->isEmpty();
    if (hasErrors and hasInterlock) {
      m_toolTip = toolTip.arg(status.errors->join("\r\n") + "\r\n" +
                              status.interlocks->join("\r\n"));
      ui->statusLabel->setStyleSheet(Status::warningRed());
      ui->statusLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
      m_iconHolder->setIcon(m_iconError);

    } else if (hasErrors and !hasInterlock) {
      m_toolTip = toolTip.arg(status.errors->join("\r\n"));
      ui->statusLabel->setStyleSheet(Status::warningRed());
      ui->statusLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
      m_iconHolder->setIcon(m_iconError);

    } else if (!hasErrors and hasInterlock) {
      m_toolTip = toolTip.arg(status.interlocks->join("\r\n"));
      ui->statusLabel->setStyleSheet(Status::warningYellow());
      ui->statusLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
      m_iconHolder->setIcon(m_iconWarning);

    } else {
      m_toolTip = "";
      ui->statusLabel->setStyleSheet(Status::warningGray());
      ui->statusLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
      m_iconHolder->setIcon(m_iconPlug);
    }
  }
}

void GroupWidgetStatus::setModel(const QString &name) {
  QString format = name;
  format.remove("Model: ");
  ui->modelLabel->setText(format);
  ui->modelLabel->setStyleSheet(Status::activeLabels());
}

void GroupWidgetStatus::setName(const QString &id) {
  ui->nameLabel->setText(id);
}

void GroupWidgetStatus::setLink(bool status) {
  if (status) {
    ui->linkLabel->setStyleSheet(Status::linkOn());
    ui->nameLabel->setStyleSheet(Status::nameLabelOn());
  } else {
    ui->linkLabel->setStyleSheet(Status::linkOff());
    ui->nameLabel->setStyleSheet(Status::nameLabelOff());
  }
}

void GroupWidgetStatus::updateStyle() {
  setStyleSheet(Status::widget());
  //  ui->modelLabel->setStyleSheet(Status::labels());
  this->update();
}

bool GroupWidgetStatus::eventFilter(QObject *obj, QEvent *event) {
  if ((obj == ui->statusLabel || obj == m_iconHolder) &&
      (event->type() == QEvent::Enter)) {
    QToolTip::showText(QCursor::pos(), m_toolTip, this);
    return true;
  }
  return QWidget::eventFilter(obj, event);
}
