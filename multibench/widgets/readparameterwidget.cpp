#include "readparameterwidget.h"
#include "ui_readparameterwidget.h"
#include "model/device/devicewidget.h"
#include "device/commandsettings.h"

ReadParameterWidget::ReadParameterWidget():
    m_command(nullptr),
    m_labelParameter(new QLabel),
    m_labelValue(new QLabel),
    m_labelUnit(new QLabel)
{


}

void ReadParameterWidget::setup(QStringView name, QSharedPointer<DevCommand> cmd){
    m_command = cmd;
    m_layout = new QHBoxLayout(this);
    this->setStyleSheet("QWidget {\
    background-color: rgb(51, 51, 51);\
    color: rgb(153,153,153);\
    font: 12pt Share Tech Mono;\
}");
    m_layout->setMargin(0);
    m_layout->setSpacing(6);
    m_labelParameter->setAlignment(Qt::AlignLeft);
    m_labelValue->setAlignment(Qt::AlignRight);
    m_labelUnit->setAlignment(Qt::AlignRight);
    m_labelParameter->setText(name.toString());

    m_labelUnit->setText(m_command->unit());

    if(m_command) {
        setValue(m_command->valueDouble(), m_command->tolerance());
        connect(m_command.get(), &DevCommand::updatedValue, this, [this](){
            setValue(m_command->valueDouble(), m_command->tolerance());
        });
        connect(m_command.get(), &DevCommand::updatedUnit, this, [this](QStringView unit){
            setUnit(unit.toString());
        });
    }

    setValue(m_command->valueDouble(), m_command->tolerance());

    m_layout->addWidget(m_labelParameter);
    m_layout->addSpacerItem(new QSpacerItem(10,20,QSizePolicy::Expanding));
    m_layout->addWidget(m_labelValue);
    m_layout->addWidget(m_labelUnit);

    this->adjustSize();
    m_labelValue->setText("    ");//резервируем место под 5 символов, чтоб виджет был нужного размера
}

// private methods

void ReadParameterWidget::setUnit(QStringView unit){
    m_unit = unit.toString();
    if(m_labelUnit)
        m_labelUnit->setText(m_unit);
}

void ReadParameterWidget::setValue(double value, int decimal){
    QString realStr = QString::number(value, 'f', decimal);
    if(m_labelValue)
        m_labelValue->setText(realStr);
}

void ReadParameterWidget::setValue(int value){
    if(m_labelValue)
        m_labelValue->setNum(value);
}

int ReadParameterWidget::getUnitslength(){
    if (m_labelUnit)
        return m_labelUnit->text().length();
    else
        return 0;
}

void ReadParameterWidget::setUnitsLength(int length){
    int charLength = 10;
    if(m_labelUnit)
        m_labelUnit->setMinimumWidth(length*charLength);
}
