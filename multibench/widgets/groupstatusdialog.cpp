#include "groupstatusdialog.h"

#include <QDebug>

#include "ui_groupstatusdialog.h"

const int WidgetsInAppearence{6};
const int WidgetStandartSize{55};

GroupStatusDialog::GroupStatusDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::GroupStatusDialog) {
  ui->setupUi(this);
  QDialog::setWindowTitle("Devices Status");
  this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
  setObjectName("GroupStatusDialog");
  m_devices.clear();
  m_devicesLayout = new QVBoxLayout(ui->scrollAreaWidget);
  //  m_devicesLayout->setMargin(0);
  m_devicesLayout->setSpacing(6);
  //  m_devicesLayout->setContentsMargins(0, 0, 0, 0);
  //  m_devicesLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_devicesLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  ui->scrollAreaWidget->setLayout(m_devicesLayout);
  GroupStatusDialog::updateStyle();
}

GroupStatusDialog::~GroupStatusDialog() { delete ui; }

void GroupStatusDialog::addDevice(quint8 addr, const QString &model) {
  if (!m_devices.contains(addr)) {
    auto device = new GroupWidgetStatus();
    device->setModel(model);
    device->setName(QString("ID: %1").arg(addr));
    m_devices.insert(addr, QSharedPointer<GroupWidgetStatus>(device));
    m_devicesLayout->addWidget(m_devices.value(addr).data());
    resizeWidget();
  }
}

void GroupStatusDialog::setStatus(QMap<quint8, DeviceStatusGroup> &status) {
  for (auto addr : status.keys()) {
    m_devices.value(addr)->addData(status[addr]);
  }
}

void GroupStatusDialog::deviceLinkChanged(int addr, bool status) {
  if (!m_devices.contains(addr)) return;
  m_devices.value(addr)->setLink(status);
}

void GroupStatusDialog::resizeWidget() {
  auto newSize = ui->scrollAreaWidget->size();
  int diffHeight = 0;
  if (m_devices.size() < WidgetsInAppearence) {
    diffHeight = m_devices.size() * (m_devices.first()->height() +
                                     m_devicesLayout->spacing()) +
                 m_devicesLayout->spacing() + WidgetStandartSize;
  } else {
    diffHeight = WidgetsInAppearence * (m_devices.first()->height() +
                                        m_devicesLayout->spacing()) +
                 m_devicesLayout->spacing() + WidgetStandartSize;
  }
  newSize.rheight() += diffHeight;
  ui->scrollArea->resize(newSize);
  auto winSize = size();
  winSize.rheight() = 83 + diffHeight;
  resize(winSize);
}

void GroupStatusDialog::updateStyle() {
  for (auto status : m_devices) {
    status->updateStyle();
  }
  this->update();
}
