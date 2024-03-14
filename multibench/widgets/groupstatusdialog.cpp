#include "groupstatusdialog.h"

#include <QDebug>

#include "ui_groupstatusdialog.h"

const int WidgetsInAppearence{6};
const int WidgetStandartSize{40};

GroupStatusDialog::GroupStatusDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::GroupStatusDialog) {
  ui->setupUi(this);
  m_devicesLayout = new QVBoxLayout(ui->scrollAreaWidget);
  m_devicesLayout->setMargin(0);
  m_devicesLayout->setSpacing(10);
  m_devicesLayout->setContentsMargins(0, 0, 0, 0);
  m_devicesLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_devicesLayout->setAlignment(Qt::AlignLeft);
  ui->scrollAreaWidget->setLayout(m_devicesLayout);
}

GroupStatusDialog::~GroupStatusDialog() { delete ui; }

void GroupStatusDialog::addDevice(quint8 addr, const QString &name,
                                  const QString &model) {
  auto device = QSharedPointer<GroupWidgetStatus>::create();
  device->setModel(model);
  device->setName(name);
  m_devicesLayout->addWidget(device.data());
  m_devices.insert(addr, device);
  resizeWidget();
}

void GroupStatusDialog::setStatus(QMap<quint8, DeviceStatusGroup> &status) {
  for (const auto &addr : status.keys()) {
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
    diffHeight =
        m_devices.size() * m_devices.first()->height() + WidgetStandartSize;
  } else {
    diffHeight =
        WidgetsInAppearence * m_devices.first()->height() + WidgetStandartSize;
  }
  newSize.rheight() += diffHeight;
  ui->scrollArea->resize(newSize);
  auto winSize = size();
  winSize.rheight() = (diffHeight > 0) ? diffHeight : 0;
  resize(winSize);
}
