#include "groupstatusdialog.h"

#include <QDebug>

#include "ui_groupstatusdialog.h"
GroupStatusDialog::GroupStatusDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::GroupStatusDialog) {
  ui->setupUi(this);
}

GroupStatusDialog::~GroupStatusDialog() { delete ui; }

void GroupStatusDialog::addDevice(quint8 addr, const QString &name,
                                  const QString &model) {
  auto device = QSharedPointer<GroupWidgetStatus>::create();
  device->setModel(model);
  device->setName(name);
  ui->devicesLayout->addWidget(device.data());
  m_devices.insert(addr, device);
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
