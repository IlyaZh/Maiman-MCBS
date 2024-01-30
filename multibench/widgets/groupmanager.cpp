#include "groupmanager.h"

#include "ui_groupmanager.h"

GroupManager::GroupManager(QMap<quint8, QString> addresses,
                           QVector<QPointer<GroupWidget>>& groups,
                           QWidget* parent)
    : QDialog(parent),
      ui(new Ui::GroupManager),
      m_addresses(addresses),
      m_groups(groups) {
  ui->setupUi(this);
  m_font.setFamily(QString::fromUtf8("Share Tech Mono"));
  m_devicesFieldLayout = new QGridLayout(ui->scrollAreaDevice);
  m_devicesFieldLayout->setMargin(0);
  m_devicesFieldLayout->setSpacing(10);
  m_devicesFieldLayout->setContentsMargins(0, 0, 0, 0);
  m_devicesFieldLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_devicesFieldLayout->setAlignment(Qt::AlignLeft);
  ui->scrollAreaDevice->setLayout(m_devicesFieldLayout);
  ui->scrollAreaDevice->setMaximumHeight(
      m_devicesFieldLayout->maximumSize().height());

  m_groupsFieldLayout = new QGridLayout(ui->scrollAreaGroup);
  m_groupsFieldLayout->setMargin(0);
  m_groupsFieldLayout->setSpacing(10);
  m_groupsFieldLayout->setContentsMargins(0, 0, 0, 0);
  m_groupsFieldLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_groupsFieldLayout->setAlignment(Qt::AlignLeft);
  ui->scrollAreaGroup->setLayout(m_groupsFieldLayout);
  ui->scrollAreaGroup->setMaximumHeight(
      m_groupsFieldLayout->maximumSize().height());

  for (auto name : m_addresses) {
    auto checkBox = new QCheckBox(this);
    checkBox->setFont(m_font);
    checkBox->setText(name);
    m_checkBoxes.insert(m_addresses.key(name), checkBox);
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
}

GroupManager::~GroupManager() { delete ui; }

void GroupManager::createGroup() {
  QSet<quint8> groupAddr;
  for (auto checkBox : m_checkBoxes) {
    if (checkBox->isChecked()) {
      checkBox->setChecked(false);
      groupAddr.insert(m_checkBoxes.key(checkBox));
    }
  }
  if (groupAddr.isEmpty()) return;
  bool createGroup = true;
  for (auto group : m_groupsTable.keys()) {
    if (m_groupsTable.value(group).groupBox_->isChecked()) {
      createGroup = false;
    }
  }
  if (createGroup) {
    auto number = m_groupsTable.count() + 1;
    emit createGroupWidget(groupAddr, number);
  } else {
    for (auto group : m_groupsTable.keys()) {
      if (m_groupsTable.value(group).groupBox_->isChecked()) {
        for (auto widget : m_groups) {
          if (widget->getGroupAddress() == group) {
            int number = group;
            m_groupsTable.value(group).groupBox_->deleteLater();
            for (auto check : m_groupsTable.value(group).subBoxes_) {
              groupAddr.insert(m_groupsTable.value(group).subBoxes_.key(check));
              check->deleteLater();
            }
            m_groupsTable.remove(group);
            emit deleteGroupWidget(widget->getAddresses());
            emit createGroupWidget(groupAddr, number);
          }
        }
      }
    }
  }
}

void GroupManager::deleteGroup() {
  for (auto group : m_groupsTable.keys()) {
    if (m_groupsTable.value(group).groupBox_->isChecked()) {
      for (auto widget : m_groups) {
        if (widget->getGroupAddress() == group) {
          m_groupsTable.value(group).groupBox_->deleteLater();
          for (auto check : m_groupsTable.value(group).subBoxes_) {
            check->deleteLater();
          }
          m_groupsTable.remove(group);
          emit deleteGroupWidget(widget->getAddresses());
        }
      }
    } else {
      for (auto sub : m_groupsTable.value(group).subBoxes_.values()) {
        if (sub->isChecked() and
            !m_groupsTable.value(group).groupBox_->isChecked()) {
          for (auto widget : m_groups) {
            if (widget->getGroupAddress() == group) {
              QSet<quint8> groupAddr;
              int number = group;
              m_groupsTable.value(group).groupBox_->deleteLater();
              for (auto check : m_groupsTable.value(group).subBoxes_) {
                if (!check->isChecked())
                  groupAddr.insert(
                      m_groupsTable.value(group).subBoxes_.key(check));
                check->deleteLater();
              }
              m_groupsTable.remove(group);

              emit deleteGroupWidget(widget->getAddresses());
              emit createGroupWidget(groupAddr, number);
            }
          }
        }
      }
    }
  }
}

void GroupManager::finishGroupAction() {
  m_groupsTable.clear();

  std::sort(m_groups.begin(), m_groups.end(),
            [](GroupWidget* a, GroupWidget* b) {
              return a->getGroupAddress() < b->getGroupAddress();
            });
  auto groupPosition = 0;
  for (auto group : m_groups) {
    auto groupCheckBox = new QCheckBox(this);
    groupsCheckBoxes groupBoxes;
    groupBoxes.groupBox_ = groupCheckBox;
    groupCheckBox->setFont(m_font);
    groupCheckBox->setText(group->getName());
    m_groupsFieldLayout->addWidget(groupCheckBox, groupPosition, 0);
    auto subgroupPosition = groupPosition;
    auto groupAddrs = group->getAddresses().values();
    std::sort(groupAddrs.begin(), groupAddrs.end());
    for (auto subGroupAddr : groupAddrs) {
      auto subGroupCheckBox = new QCheckBox(this);
      subGroupCheckBox->setFont(m_font);
      subGroupCheckBox->setText(m_addresses.value(subGroupAddr));
      groupBoxes.subBoxes_.insert(subGroupAddr, subGroupCheckBox);
      subgroupPosition++;
      m_groupsFieldLayout->addWidget(subGroupCheckBox, subgroupPosition, 1);
    }
    m_groupsTable.insert(group->getGroupAddress(), groupBoxes);
    groupPosition += group->getAddresses().size() + 1;
  }
  paintDevices();
}

void GroupManager::paintDevices() {
  for (auto check : m_checkBoxes) {
    check->show();
  }
  for (auto group : m_groupsTable) {
    for (auto subGroupKey : group.subBoxes_.keys()) {
      if (m_checkBoxes.contains(subGroupKey)) {
        m_checkBoxes.value(subGroupKey)->hide();
      }
    }
  }
}
