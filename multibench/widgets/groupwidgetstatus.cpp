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
}

GroupWidgetStatus::~GroupWidgetStatus() { delete ui; }

void GroupWidgetStatus::addData(DeviceStatusGroup &status) {
  //  QString format(R"(<span style=color:'%1'>Warning</span>)");
  QString toolTip(
      R"(<span style='background-color: #FFFFFF; color: #000000'>%1</span>)");
  bool hasErrors = !status.errors->isEmpty();
  bool hasInterlock = !status.interlocks->isEmpty();
  if (hasErrors and hasInterlock) {
    ui->statusLabel->setToolTip(toolTip.arg(status.errors->join("\n") + "\n" +
                                            status.interlocks->join("\n")));
    ui->statusLabel->setStyleSheet(Status::warningRed());
    ui->statusLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_iconHolder->setToolTip(toolTip.arg(status.errors->join("\n") + "\n" +
                                         status.interlocks->join("\n")));
    m_iconHolder->setIcon(m_iconError);

  } else if (hasErrors and !hasInterlock) {
    ui->statusLabel->setToolTip(toolTip.arg(status.errors->join("\n")));
    ui->statusLabel->setStyleSheet(Status::warningRed());
    ui->statusLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_iconHolder->setToolTip(toolTip.arg(status.errors->join("\n")));
    m_iconHolder->setIcon(m_iconError);

  } else if (!hasErrors and hasInterlock) {
    ui->statusLabel->setToolTip(toolTip.arg(status.interlocks->join("\n")));
    ui->statusLabel->setStyleSheet(Status::warningYellow());
    ui->statusLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_iconHolder->setToolTip(toolTip.arg(status.interlocks->join("\n")));
    m_iconHolder->setIcon(m_iconWarning);

  } else {
    ui->statusLabel->setToolTip(QString());
    ui->statusLabel->setStyleSheet(Status::warningGray());
    ui->statusLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_iconHolder->setToolTip(QString());
    m_iconHolder->setIcon(m_iconPlug);
  }

  if (status.devStarted.has_value()) {
    for (auto map = status.devStarted->cbegin(),
              end = status.devStarted->cend();
         map != end; ++map) {
      if (!m_devs.contains(map.key())) {
        auto devLabel = new QLabel();
        devLabel->setStyleSheet(Status::activeLabels());
        devLabel->setFont(QFont("Poppins", 14));
        devLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        devLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        auto nameLabel = new QLabel(map.key() + ": ");
        nameLabel->setStyleSheet(Status::unactiveLabels());
        nameLabel->setFont(QFont("Poppins", 14));
        nameLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        nameLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        if (map.key() == "Laser") {
          devLabel->setFixedSize(39, 24);
          nameLabel->setFixedSize(73, 24);
          laserLayout->addWidget(nameLabel);
          laserLayout->addWidget(devLabel);
          laserLayout->addSpacerItem(new QSpacerItem(
              0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));
        } else if (map.key() == "TEC") {
          nameLabel->setFixedSize(66, 24);
          devLabel->setFixedSize(46, 24);
          tecLayout->addWidget(nameLabel);
          tecLayout->addWidget(devLabel);
          tecLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding,
                                                   QSizePolicy::Fixed));
        }
        m_devs.insert(map.key(), devLabel);
        //        QWidget *cell = new QWidget;
        //        cell->setFixedSize(84, 24);  // Установи фиксированный размер
        //        ячейки auto devLabel = new QLabel(map.key(), cell);
        //        devLabel->setStyleSheet(StyleStorage::Group::Status::labels());
        //        devLabel->setFont(QFont("Poppins", 14));
        //        devLabel->setSizePolicy(QSizePolicy::Fixed,
        //        QSizePolicy::Fixed); devLabel->setAlignment(Qt::AlignHCenter |
        //        Qt::AlignLeft); devLabel->setFixedSize(84, 24); QVBoxLayout
        //        *cellLayout = new QVBoxLayout(cell);
        //        cellLayout->addWidget(devLabel);
        //        cellLayout->setContentsMargins(0, 0, 0, 0);  // Убираем
        //        отступы cellLayout->setAlignment(Qt::AlignLeft);
        //        ui->horizontalLayout->addWidget(cell);
        //        m_devs.insert(map.key(), devLabel);
      }
      if (map.value()) {
        m_devs.value(map.key())->setText(QString("ON "));
      } else {
        m_devs.value(map.key())->setText(QString("OFF"));
      }
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
