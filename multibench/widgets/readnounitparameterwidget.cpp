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
    background-color: rgb(51, 51, 51);\
    color: rgb(153,153,153);\
    font: 12pt Share Tech Mono;\
}");
  m_layout->setMargin(0);
  m_layout->setSpacing(6);
  m_labelParameter = new QLabel;
  m_labelValue = new QLabel;
  m_labelUnit = new QLabel;
  m_labelParameter->setAlignment(Qt::AlignLeft);
  m_labelValue->setAlignment(Qt::AlignRight);
  m_labelUnit->setAlignment(Qt::AlignRight);
  m_labelParameter->setText(name.toString());

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

  this->adjustSize();
}
