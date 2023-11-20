#include "binarywidget.h"

#include "device/commandsettings.h"
#include "ui_binarywidget.h"

BinaryWidget::BinaryWidget(const Checkbox &settings,
                           QSharedPointer<DevCommand> cmd, QWidget *parent)
    : GuiWidgetBase(parent),
      ui(new Ui::BinaryWidget),
      m_settings(settings),
      m_cmd(cmd) {
  ui->setupUi(this);
  ui->statusCheckbox->setText(m_settings.name);

  if (cmd) {
    setValue(cmd->valueInt());
    connect(cmd.get(), &DevCommand::updatedValue, this,
            [this]() { setValue(m_cmd->valueInt()); });
  }

  connect(ui->statusCheckbox, &QCheckBox::clicked, this,
          &BinaryWidget::checkBoxClicked);
}

BinaryWidget::BinaryWidget(const Checkbox &settings,
                           QSharedPointer<CommandConverter> converter,
                           QWidget *parent)
    : GuiWidgetBase(parent),
      ui(new Ui::BinaryWidget),
      m_settings(settings),
      m_converter(converter) {
  ui->setupUi(this);
  ui->statusCheckbox->setText(m_settings.name);

  if (converter) {
    setValue(converter->valueInt());
    //    connect(cmd.get(), &DevCommand::updatedValue, this,
    //            [this]() { setValue(m_cmd->valueInt()); });
  }

  connect(ui->statusCheckbox, &QCheckBox::clicked, this,
          &BinaryWidget::checkBoxClicked);
}

BinaryWidget::~BinaryWidget() { delete ui; }

void BinaryWidget::checkBoxClicked(bool checked) {
  emit setDataFromWidget(
      m_converter->code(),
      m_converter->getRawFromValue(static_cast<double>(
          (checked) ? m_settings.onCommand : m_settings.offCommand)));
  //  m_cmd->setFromWidget((checked) ? m_settings.onCommand
  //                                 : m_settings.offCommand);
}

// private methods

void BinaryWidget::setValue(quint16 value) {
  ui->statusCheckbox->setChecked((value & m_settings.mask) != 0);
}

void BinaryWidget::setData(quint16 code, quint16 data) {
  if (code != m_converter->code()) return;
  m_converter->setValue(data);
  setValue(m_converter->valueInt());
}

QVector<quint16> BinaryWidget::Subscribe() {
  m_codes.append(m_converter->code());
  return m_codes;
}
