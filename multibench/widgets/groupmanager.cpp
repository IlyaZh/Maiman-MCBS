#include "groupmanager.h"

#include "ui_groupmanager.h"

GroupManager::GroupManager(QSet<quint8> addresses, QWidget *parent)
    : QDialog(parent), ui(new Ui::GroupManager), m_addresses(addresses) {
  ui->setupUi(this);
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

  for (auto addr : m_addresses) {
    auto checkBox = new QCheckBox(this);
    checkBox->setText(QString("ID:%1").arg(addr));
    m_devicesFieldLayout->addWidget(checkBox);
    m_checkBoxes.insert(addr, checkBox);
  }

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
      checkBox->setVisible(false);
      groupAddr.insert(m_checkBoxes.key(checkBox));
      checkBox->setChecked(false);
    }
  }
  if (groupAddr.isEmpty()) return;
  emit createGroupWidget(groupAddr);
  auto group = new QCheckBox(this);
  auto number = m_groupCheckBoxes.count() + 1;
  group->setText(QString("Group %1").arg(number));
  m_groupsFieldLayout->addWidget(group);
  m_groupCheckBoxes.insert(group, groupAddr);
}

void GroupManager::deleteGroup() {
  for (auto check : m_groupCheckBoxes.keys()) {
    if (check->isChecked()) {
      emit deleteGroupWidget(m_groupCheckBoxes.value(check));
      for (auto addr : m_groupCheckBoxes.value(check)) {
        m_checkBoxes.value(addr)->setVisible(true);
        m_checkBoxes.value(addr)->setChecked(false);
      }
      m_groupsFieldLayout->removeWidget(check);
      m_groupCheckBoxes.remove(check);
    }
  }
}
