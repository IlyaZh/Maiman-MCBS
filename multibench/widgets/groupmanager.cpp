#include "groupmanager.h"

#include "ui_groupmanager.h"

GroupManager::GroupManager(const QMap<quint8, QPointer<DeviceHolder>>& devices,
                           const QMap<int, QPointer<GroupWidget>>& groups,
                           QWidget* parent)
    : QDialog(parent),
      ui(new Ui::GroupManager),
      m_devices(devices),
      m_groups(groups),
      m_buttonGroup(new QButtonGroup(this)) {
  this->setModal(true);
  QDialog::setWindowTitle("Group Manager");
  this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
  ui->setupUi(this);
  ui->verticalLayout->setAlignment(Qt::AlignHCenter);
  //  m_font.setFamily(QString::fromUtf8("Poppins"));
  //  m_font.setPointSize(11);
  //  m_font.setLetterSpacing(QFont::PercentageSpacing, 105);
  m_buttonGroup->setExclusive(false);
  m_devicesFieldLayout = new QVBoxLayout(ui->scrollAreaDevice);
  m_devicesFieldLayout->setSpacing(6);
  m_devicesFieldLayout->setContentsMargins(18, 15, 0, 0);
  m_devicesFieldLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_devicesFieldLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  ui->scrollAreaDevice->setLayout(m_devicesFieldLayout);
  ui->scrollAreaDevice->setObjectName("devs");
  ui->scrollAreaGroup->setObjectName("groups");
  //  ui->scrollAreaDevice->setMaximumHeight(
  //      m_devicesFieldLayout->maximumSize().height());

  m_groupsFieldLayout = new QVBoxLayout(ui->scrollAreaGroup);
  m_groupsFieldLayout->setSpacing(6);
  m_groupsFieldLayout->setContentsMargins(18, 15, 0, 0);
  m_groupsFieldLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_groupsFieldLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  ui->scrollAreaGroup->setLayout(m_groupsFieldLayout);
  //  ui->scrollAreaGroup->setMaximumHeight(
  //      m_groupsFieldLayout->maximumSize().height());

  for (const auto& devs : m_devices) {
    auto checkBox = new QCheckBox(this);
    checkBox->setFont(m_font);
    checkBox->setText(devs->getName());
    checkBox->setStyleSheet(StyleStorage::Group::Manager::checkBoxes());
    auto device = QSharedPointer<deviceCheckBox>::create();
    device->d_addr = devs->getAddress();
    device->d_checkBox = checkBox;
    device->d_isDeleted = false;
    m_devicesContainer.insert(devs->getAddress(), device);
    m_devicesFieldLayout->addWidget(checkBox);

    connect(device->d_checkBox, &QCheckBox::clicked, this,
            [this, device](bool checked) {
              GroupManager::checkBoxClicked(checked, device->d_addr);
            });
  }

  if (!m_groups.isEmpty()) {
    for (auto& group : m_groups) {
      createOneGroup(group->getGroupAddress(), group->getAddresses(),
                     group->getName());
    }
  }

  connect(ui->CreateGroup, &QPushButton::clicked, this,
          &GroupManager::addDeviceToGroup);
  connect(ui->DeleteGroup, &QPushButton::clicked, this,
          &GroupManager::removeDeviceFromGroup);
  connect(m_buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
          SLOT(groupButtonClicked(QAbstractButton*)));
  connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
          &GroupManager::accept);
  connect(ui->buttonBox, &QDialogButtonBox::rejected, this,
          &GroupManager::reject);
  connect(this, &GroupManager::finished, this, [this](int result) {
    switch (result) {
      case QDialog::Accepted: {
        emit sendAllGroups(m_groupsContainer);
        deleteLater();
      } break;
      case QDialog::Rejected: {
        deleteLater();
      } break;
    }
  });
  GroupManager::updateStyle();
}

GroupManager::~GroupManager() { delete ui; }

void GroupManager::groupButtonClicked(QAbstractButton* button) {
  bool state = button->isChecked();
  for (auto& check : m_buttonGroup->buttons()) {
    check->setChecked(false);
  }
  if (state) button->setChecked(true);
}

void GroupManager::addDeviceToGroup() {
  QSet<quint8> groupAddr;
  for (auto& device : m_devicesContainer) {
    if (device->d_checkBox->isChecked()) {
      device->d_checkBox->setChecked(false);
      m_devicesFieldLayout->removeWidget(device->d_checkBox);
      groupAddr.insert(device->d_addr);
    }
  }
  if (groupAddr.isEmpty()) return;

  if (m_buttonGroup->checkedButton() == 0) {
    auto number = 1;
    if (!m_groupsContainer.isEmpty()) {
      auto list = m_groupsContainer.keys();
      for (auto index = list.begin(), end = list.end(); index != end; ++index) {
        if (*index == number) number++;
      }
    }
    createOneGroup(number, groupAddr);
  } else {
    for (auto& group : m_groupsContainer) {
      if (group->g_checkBox->isChecked()) {
        for (auto& addr : qAsConst(groupAddr)) {
          group->g_subBoxes.insert(addr, m_devicesContainer.take(addr));
          group->g_layout->addWidget(group->g_subBoxes.value(addr)->d_checkBox);
        }
        m_groupsFieldLayout->update();
        group->g_checkBox->setChecked(false);
      }
    }
  }
  sortWidgets();
}

void GroupManager::removeDeviceFromGroup() {
  if (m_buttonGroup->checkedButton() != 0) {
    for (auto& group : m_groupsContainer) {
      if (group->g_checkBox->isChecked()) {
        group->g_isDeleted = true;
        auto addrs = group->g_subBoxes.keys();
        auto g = group->g_subBoxes.values();
        for (auto& device : g) {
          group->g_layout->removeWidget(device->d_checkBox);
        }
        for (auto device : addrs) {
          m_devicesContainer.insert(device, group->g_subBoxes.take(device));
          m_devicesFieldLayout->addWidget(
              m_devicesContainer.value(device)->d_checkBox);
          m_devicesContainer.value(device)->d_checkBox->setChecked(false);
        }
      }
    }
  } else {
    for (auto& group : m_groupsContainer) {
      auto groups = group->g_subBoxes.values();
      for (auto& subBoxes : groups) {
        if (subBoxes->d_checkBox->isChecked()) {
          if (group->g_subBoxes.size() == 1) {
            group->g_isDeleted = true;
          } else {
            auto addr = subBoxes->d_addr;
            group->g_layout->removeWidget(subBoxes->d_checkBox);
            m_devicesContainer.insert(addr, group->g_subBoxes.take(addr));
            m_devicesFieldLayout->addWidget(
                m_devicesContainer.value(addr)->d_checkBox);
            m_devicesContainer.value(addr)->d_checkBox->setChecked(false);
          }
        }
      }
    }
  }
  if (!m_groupsContainer.isEmpty()) {
    QSet<quint8> deleted;
    for (auto& group : m_groupsContainer) {
      if (group->g_isDeleted == true) {
        deleted.insert(group->g_addr);
        m_groupsFieldLayout->removeItem(group->g_layout);
        m_groupsFieldLayout->removeWidget(group->g_checkBox);
        int id = m_buttonGroup->id(group->g_checkBox);
        m_buttonGroup->button(id)->setChecked(false);
        m_buttonGroup->removeButton(group->g_checkBox);
        if (group->g_subBoxes.size() == 1) {
          auto sub = group->g_subBoxes.firstKey();
          group->g_layout->removeWidget(
              group->g_subBoxes.value(sub)->d_checkBox);
          m_devicesContainer.insert(sub, group->g_subBoxes.take(sub));
          m_devicesFieldLayout->addWidget(
              m_devicesContainer.value(sub)->d_checkBox);
          m_devicesContainer.value(sub)->d_checkBox->setChecked(false);
        }
      }
    }
    for (auto deletedGroup : deleted) {
      m_groupsContainer.value(deletedGroup)->g_checkBox->deleteLater();
      m_groupsContainer.value(deletedGroup)->g_layout->deleteLater();
      m_groupsContainer.value(deletedGroup)->g_subBoxes.clear();
      m_groupsContainer.remove(deletedGroup);
    }
  }
  sortWidgets();
}

void GroupManager::createOneGroup(int g_addr, const QSet<quint8> devicesAddrs,
                                  const QString& name) {
  auto groupCheckBox = new QCheckBox(this);
  groupCheckBox->setStyleSheet(StyleStorage::Group::Manager::checkBoxes());
  auto groupBoxes = QSharedPointer<groupCheckBoxes>::create();
  groupBoxes->g_checkBox = groupCheckBox;
  groupBoxes->g_addr = g_addr;
  groupBoxes->g_checkBox->setFont(m_font);
  auto g_name = (name == "") ? QString("Group %1").arg(g_addr) : name;
  groupBoxes->g_checkBox->setText(g_name);
  m_buttonGroup->addButton(groupBoxes->g_checkBox);
  m_groupsFieldLayout->addWidget(groupBoxes->g_checkBox);
  auto groupAddrs = devicesAddrs.values();
  groupBoxes->g_layout = new QVBoxLayout();
  groupBoxes->g_layout->setContentsMargins(21, 0, 0, 0);
  std::sort(groupAddrs.begin(), groupAddrs.end());
  for (auto subGroupAddr : groupAddrs) {
    groupBoxes->g_subBoxes.insert(subGroupAddr,
                                  m_devicesContainer.take(subGroupAddr));
    groupBoxes->g_layout->addWidget(
        groupBoxes->g_subBoxes.value(subGroupAddr)->d_checkBox);
    groupBoxes->g_subBoxes.value(subGroupAddr)->d_checkBox->setChecked(false);
  }
  m_groupsFieldLayout->addLayout(groupBoxes->g_layout);
  m_groupsContainer.insert(g_addr, groupBoxes);
}

void GroupManager::updateStyle() {
  ui->scrollAreaDevice->setStyleSheet(
      StyleStorage::Group::Manager::devsField());
  ui->scrollAreaDevice->update();
  ui->scrollAreaGroup->setStyleSheet(
      StyleStorage::Group::Manager::groupsField());
  ui->scrollAreaGroup->update();
  this->update();
}

void GroupManager::sortWidgets() {
  for (auto& dev : m_devicesContainer) {
    m_devicesFieldLayout->removeWidget(dev->d_checkBox);
  }
  for (auto& dev : m_devicesContainer) {
    m_devicesFieldLayout->addWidget(dev->d_checkBox);
  }
  for (auto& group : m_groupsContainer) {
    m_groupsFieldLayout->removeWidget(group->g_checkBox);
    m_groupsFieldLayout->removeItem(group->g_layout);
    for (auto& dev : group->g_subBoxes) {
      group->g_layout->removeWidget(dev->d_checkBox);
    }
  }
  for (auto& group : m_groupsContainer) {
    m_groupsFieldLayout->addWidget(group->g_checkBox);
    m_groupsFieldLayout->addLayout(group->g_layout);
    for (auto& dev : group->g_subBoxes) {
      group->g_layout->addWidget(dev->d_checkBox);
    }
  }
}

void GroupManager::checkBoxClicked(bool status, int addr) {
  if (status) {
    if (m_devicesContainer.contains(addr)) {
      for (auto& group : m_groupsContainer) {
        for (auto& dev : group->g_subBoxes) {
          dev->d_checkBox->setChecked(false);
        }
      }
    } else {
      for (auto& group : m_groupsContainer) {
        if (!group->g_subBoxes.contains(addr)) {
          for (auto& dev : group->g_subBoxes) {
            dev->d_checkBox->setChecked(false);
          }
        }
      }
      for (auto& dev : m_devicesContainer) {
        dev->d_checkBox->setChecked(false);
      }
    }
  }
}
