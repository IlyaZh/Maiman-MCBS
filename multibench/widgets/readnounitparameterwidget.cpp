#include "readnounitparameterwidget.h"

#include "device/commandsettings.h"
#include "model/device/devicewidget.h"

ReadNoUnitParameterWidget::ReadNoUnitParameterWidget() {}

void ReadNoUnitParameterWidget::setup(QStringView name,
                                      QSharedPointer<DevCommand> cmd) {
  m_command = cmd;
  m_layout = new QHBoxLayout(this);
  this->setStyleSheet(
      "QWidget {\
    background-color: #282828;\
    color: rgb(153,153,153);\
}");
  QFont font12("Poppins", 10);
  font12.setLetterSpacing(QFont::PercentageSpacing, 105);
  font12.setPixelSize(16);
  setFont(font12);
  m_layout->setMargin(0);
  m_layout->setSpacing(6);
  m_labelParameter = new QLabel;
  m_labelValue = new QLabel;
  m_labelUnit = new QLabel;
  m_labelParameter->setAlignment(Qt::AlignLeft);
  m_labelValue->setAlignment(Qt::AlignRight);
  m_labelUnit->setAlignment(Qt::AlignRight);
  m_labelParameter->setText(name.toString());
  m_labelParameter->setFont(font12);
  m_labelValue->setFont(font12);
  m_labelUnit->setFont(font12);
  m_labelUnit->setText(m_command->unit());

  if (m_command) {
    setValue(m_command->valueDouble(), m_command->tolerance());
    connect(m_command.get(), &DevCommand::updatedValue, this, [this]() {
      setUnit(QString::number(m_command->valueDouble()));
      setUnitsLength(getUnitslength());
    });
  }

  setValue(m_command->valueDouble(), m_command->tolerance());
  m_labelValue->setText("");

  m_layout->addWidget(m_labelParameter);
  m_layout->addSpacerItem(new QSpacerItem(0, 20, QSizePolicy::Expanding));
  m_layout->addWidget(m_labelValue);
  m_layout->addWidget(m_labelUnit);

  //  this->adjustSize();
}

void ReadNoUnitParameterWidget::setup(QStringView name,
                                      QSharedPointer<CommandConverter> cmd) {
  m_converter = cmd;
  m_layout = new QHBoxLayout(this);
  this->setStyleSheet(
      "QWidget {\
    background-color: #282828;\
    color: rgb(153,153,153);\
}");
  QFont font12("Poppins", 10);
  font12.setLetterSpacing(QFont::PercentageSpacing, 105);
  font12.setPixelSize(16);
  setFont(font12);
  m_layout->setMargin(0);
  m_layout->setSpacing(6);
  m_labelParameter = new QLabel;
  m_labelValue = new QLabel;
  m_labelUnit = new QLabel;
  m_labelParameter->setAlignment(Qt::AlignLeft);
  m_labelValue->setAlignment(Qt::AlignRight);
  m_labelUnit->setAlignment(Qt::AlignRight);
  m_labelParameter->setText(name.toString());
  QFont font11("Poppins", 11);
  font11.setLetterSpacing(QFont::PercentageSpacing, 105);
  font11.setPixelSize(14);
  m_labelParameter->setFont(font11);
  m_labelValue->setFont(font11);
  m_labelUnit->setFont(font11);
  m_labelUnit->setText(m_converter->unit());

  if (m_converter) {
    setValue(m_converter->valueDouble(), m_converter->tolerance());
  }

  setValue(m_converter->valueDouble(), m_converter->tolerance());
  m_labelValue->setText("");

  m_layout->setAlignment(Qt::AlignVCenter);
  this->setMaximumSize(300, 20);
  this->setMaximumSize(300, 20);
  this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum));
  m_labelParameter->setMaximumSize(210, 20);
  m_labelParameter->setMaximumSize(210, 20);
  m_labelParameter->setSizePolicy(
      QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
  m_labelValue->setMaximumSize(54, 20);
  m_labelValue->setMaximumSize(54, 20);
  m_labelValue->setSizePolicy(
      QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum));
  m_labelUnit->setMaximumSize(24, 20);
  m_labelUnit->setMaximumSize(24, 20);
  m_labelUnit->setSizePolicy(
      QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum));

  m_layout->addWidget(m_labelParameter);
  m_layout->addSpacerItem(new QSpacerItem(0, 20, QSizePolicy::Expanding));
  m_layout->addWidget(m_labelValue);
  m_layout->addWidget(m_labelUnit);

  this->adjustSize();
}

void ReadNoUnitParameterWidget::setData(quint16 code, quint16 data) {
  if (m_converter->code() == code) {
    m_converter->setValue(data);
    setUnit(QString::number(m_converter->valueDouble()));
    setUnitsLength(getUnitslength());
  }
}
