#include "binarywidget.h"
#include "ui_binarywidget.h"
#include "device/commandsettings.h"
#include <QDebug>

BinaryWidget::BinaryWidget(const Checkbox &settings, QSharedPointer<DevCommand> cmd, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BinaryWidget),
    m_settings(settings),
    m_cmd(cmd)
{
    ui->setupUi(this);
    ui->statusCheckbox->setText(m_settings.name);

    if(cmd) {
        setValue(cmd->valueUInt());
        connect(cmd.get(), &DevCommand::updatedValue, this, [this](){
            setValue(m_cmd->valueUInt());
        });
    }

    connect(ui->statusCheckbox, &QCheckBox::clicked, this, &BinaryWidget::checkBoxClicked);
}

BinaryWidget::~BinaryWidget()
{
    delete ui;
}

void BinaryWidget::checkBoxClicked(bool checked) {
    m_cmd->sendValue((checked) ? m_settings.onCommand : m_settings.offCommand);
    ui->statusCheckbox->setCheckState(Qt::CheckState::PartiallyChecked);
}

// private methods

void BinaryWidget::setValue(quint16 value) {
    ui->statusCheckbox->setChecked((value & m_settings.mask) != 0);
}
