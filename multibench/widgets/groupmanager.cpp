#include "groupmanager.h"

#include "ui_groupmanager.h"

GroupManager::GroupManager(QMap<quint8, QPointer<DeviceWidget>>& devices,
                           QMap<int, QPointer<GroupWidget>>& groups,
                           QWidget* parent)
    : QDialog(parent),
      ui(new Ui::GroupManager),
      m_devices(devices),
      m_groups(groups),
      m_buttonGroup(new QButtonGroup(this)) {
  ui->setupUi(this);
  m_font.setFamily(QString::fromUtf8("Share Tech Mono"));
  m_buttonGroup->setExclusive(false);
  m_devicesFieldLayout = new QVBoxLayout(ui->scrollAreaDevice);
  m_devicesFieldLayout->setSpacing(10);
  m_devicesFieldLayout->setContentsMargins(10, 10, 0, 0);
  m_devicesFieldLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_devicesFieldLayout->setAlignment(Qt::AlignLeft);
  ui->scrollAreaDevice->setLayout(m_devicesFieldLayout);
  ui->scrollAreaDevice->setMaximumHeight(
      m_devicesFieldLayout->maximumSize().height());

  m_groupsFieldLayout = new QVBoxLayout(ui->scrollAreaGroup);
  m_groupsFieldLayout->setSpacing(10);
  m_groupsFieldLayout->setContentsMargins(10, 10, 0, 0);
  m_groupsFieldLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_groupsFieldLayout->setAlignment(Qt::AlignLeft);
  ui->scrollAreaGroup->setLayout(m_groupsFieldLayout);
  ui->scrollAreaGroup->setMaximumHeight(
      m_groupsFieldLayout->maximumSize().height());

  for (const auto& devs : m_devices) {
    auto checkBox = new QCheckBox(this);
    checkBox->setFont(m_font);
    checkBox->setText(devs->getName());
    m_checkBoxes.insert(devs->getAddress(), checkBox);
    m_devicesFieldLayout->addWidget(checkBox);
  }

  if (!m_groups.isEmpty()) {
    finishGroupAction();
  } else
    paintDevices();

  connect(ui->CreateGroup, &QPushButton::clicked, this,
          &GroupManager::createGroup);
  connect(ui->DeleteGroup, &QPushButton::clicked, this,
          &GroupManager::deleteGroup);
  connect(m_buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
          SLOT(groupButtonClicked(QAbstractButton*)));
}

GroupManager::~GroupManager() { delete ui; }

void GroupManager::createGroup() {
  QSet<quint8> groupAddr;
  for (auto& checkBox : m_checkBoxes) {
    if (checkBox->isChecked()) {
      checkBox->setChecked(false);
      groupAddr.insert(m_checkBoxes.key(checkBox));
    }
  }
  if (groupAddr.isEmpty()) return;

  if (m_buttonGroup->checkedButton() == nullptr) {
    auto number = 1;
    for (const auto& group : m_groups) {
      if (group->getGroupAddress() == number) number++;
    }
    emit createGroupWidget(groupAddr, number);
  } else {
    for (auto& group : m_groupsTable) {
      if (group->groupBox_->isChecked()) {
        for (auto& addr : groupAddr) {
          auto subGroupCheckBox = new QCheckBox(this);
          subGroupCheckBox->setFont(m_font);
          subGroupCheckBox->setText(m_devices.value(addr)->getName());
          group->subBoxes_.insert(m_devices.value(addr)->getAddress(),
                                  subGroupCheckBox);
          group->layout_->addWidget(subGroupCheckBox);
          emit addMemberGroup(group->groupAddr_, addr);
        }
      }
    }
  }
}

void GroupManager::deleteGroup() {
  if (m_buttonGroup->checkedButton() != nullptr) {
    for (const auto& group : m_groupsTable.values()) {
      if (group->groupBox_->isChecked()) {
        clearGroup(group);
      }
    }
  } else {
    QSharedPointer<groupsCheckBoxes> deletedGroup = nullptr;
    for (const auto& group : m_groupsTable) {
      for (auto& check : group->subBoxes_.keys()) {
        if (group->subBoxes_.value(check)->isChecked()) {
          if (group->subBoxes_.size() == 1) {
            deletedGroup = group;
          } else {
            group->layout_->removeWidget(group->subBoxes_.value(check));
            group->subBoxes_.value(check)->deleteLater();
            emit removeMemberGroup(group->groupAddr_, check);
            m_checkBoxes.value(check)->show();
          }
        }
      }
    }
    if (deletedGroup != nullptr) clearGroup(deletedGroup);
  }
}

void GroupManager::clearGroup(QSharedPointer<groupsCheckBoxes> group) {
  emit deleteGroupWidget(m_groups.value(group->groupAddr_)->getGroupAddress());
}

void GroupManager::finishGroupAction() {
  auto groupKeys = m_groupsTable.keys();
  for (auto& deletedGroup : groupKeys) {
    if (!m_groups.contains(deletedGroup)) {
      m_buttonGroup->removeButton(m_groupsTable.value(deletedGroup)->groupBox_);
      m_groupsFieldLayout->removeWidget(
          m_groupsTable.value(deletedGroup)->groupBox_);
      m_groupsTable.value(deletedGroup)->groupBox_->deleteLater();
      for (auto& check : m_groupsTable.value(deletedGroup)->subBoxes_) {
        check->deleteLater();
      }
      m_groupsFieldLayout->removeItem(
          m_groupsTable.value(deletedGroup)->layout_);
      m_groupsTable.value(deletedGroup)->layout_->deleteLater();
      m_groupsTable.remove(deletedGroup);
    } else {
      auto subGroupsKeys = m_groupsTable.value(deletedGroup)->subBoxes_.keys();
      for (auto& addr : subGroupsKeys) {
        if (!m_groups.value(deletedGroup)->getAddresses().contains(addr)) {
          m_groupsTable[deletedGroup]->subBoxes_.remove(addr);
        }
      }
    }
  }

  for (auto& group : m_groups) {
    if (!m_groupsTable.contains(group->getGroupAddress())) {
      auto groupCheckBox = new QCheckBox(this);
      QSharedPointer<groupsCheckBoxes> groupBoxes =
          QSharedPointer<groupsCheckBoxes>(new groupsCheckBoxes());
      groupBoxes->groupBox_ = groupCheckBox;
      groupBoxes->groupAddr_ = group->getGroupAddress();
      groupCheckBox->setFont(m_font);
      groupCheckBox->setText(group->getName());
      m_buttonGroup->addButton(groupCheckBox);
      m_groupsFieldLayout->addWidget(groupCheckBox);
      auto groupAddrs = group->getAddresses().values();
      groupBoxes->layout_ = new QVBoxLayout();
      groupBoxes->layout_->setContentsMargins(30, 0, 0, 0);
      std::sort(groupAddrs.begin(), groupAddrs.end());
      for (auto subGroupAddr : groupAddrs) {
        auto subGroupCheckBox = new QCheckBox(this);
        subGroupCheckBox->setFont(m_font);
        subGroupCheckBox->setText(m_devices.value(subGroupAddr)->getName());
        groupBoxes->subBoxes_.insert(subGroupAddr, subGroupCheckBox);
        groupBoxes->layout_->addWidget(subGroupCheckBox);
      }
      m_groupsFieldLayout->addLayout(groupBoxes->layout_);
      m_groupsTable.insert(group->getGroupAddress(), groupBoxes);
    }
  }
  paintDevices();
}

void GroupManager::paintDevices() {
  for (auto& check : m_checkBoxes) {
    check->show();
  }
  for (auto& group : m_groupsTable) {
    for (auto& subGroupKey : group->subBoxes_.keys()) {
      if (m_checkBoxes.contains(subGroupKey)) {
        m_checkBoxes.value(subGroupKey)->hide();
      }
    }
  }
}

void GroupManager::groupButtonClicked(QAbstractButton* button) {
  bool state = button->isChecked();
  for (auto& check : m_buttonGroup->buttons()) {
    check->setChecked(false);
  }
  if (state)
    button->setChecked(true);
  else
    button->setChecked(false);
}
