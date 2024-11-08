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
  //  ui->statusCheckbox->setIconSize(QSize(12, 12));
  //  QIcon icon1;
  //  icon1.addFile(QString::fromUtf8(":/resources/images/checkBoxOff.png"),
  //                QSize(), QIcon::Normal, QIcon::Off);
  //  icon1.addFile(QString::fromUtf8(":/resources/images/checkBoxOn.png"),
  //  QSize(),
  //                QIcon::Normal, QIcon::On);
  //  ui->statusCheckbox->setIcon(icon1);
  QFont font11("Poppins", 11);
  font11.setLetterSpacing(QFont::PercentageSpacing, 105);
  ui->statusCheckbox->setFont(font11);
  if (converter) {
    setValue(converter->valueInt());
    //    connect(cmd.get(), &DevCommand::updatedValue, this,
    //            [this]() { setValue(m_cmd->valueInt()); });
  }

  connect(ui->statusCheckbox, &QCheckBox::clicked, this,
          &BinaryWidget::checkBoxClicked);
  updateStyle();
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

void BinaryWidget::updateStyle() {
  this->setStyleSheet(StyleStorage::BinaryWidget::widget());
  ui->statusCheckbox->setStyleSheet(StyleStorage::BinaryWidget::checkBox());
  ui->statusCheckbox->update();
  this->update();
}
