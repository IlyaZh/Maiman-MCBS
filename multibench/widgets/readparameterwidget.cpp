#include "readparameterwidget.h"
#include "ui_readparameterwidget.h"
#include "model/device/devicewidget.h"
#include "device/commandsettings.h"

ReadParameterWidget::ReadParameterWidget()
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
    m_labelParameter = new QLabel;
    m_labelValue = new QLabel;
    m_labelUnit = new QLabel;
    m_labelParameter->setAlignment(Qt::AlignLeft);
    m_labelValue->setAlignment(Qt::AlignRight);
    m_labelUnit->setAlignment(Qt::AlignRight);
    m_labelParameter->setText(name.toString());

    m_labelUnit->setText(m_command->unit());

    if(m_command) {
        setValue(m_command->valueDouble(), m_command->tolerance());
        connect(m_command.get(), &DevCommand::updatedValue, this, [this](){
            if (m_command->code() == 3){
                setUnit(QString::number(m_command->valueDouble()));
                setUnitsLength(getUnitslength());
            }
            else
                setValue(m_command->valueDouble(), m_command->tolerance());
        });
        connect(m_command.get(), &DevCommand::updatedUnit, this, [this](QStringView unit){
            if (m_command->code() == 3)
                m_labelValue->setText("");
            else
                setUnit(unit.toString());
        });
    }

    /*setValue(m_settings.real, value);*/
    setValue(m_command->valueDouble(), m_command->tolerance());
    if (m_command->code() == 3)
        m_labelValue->setText("");

    m_layout->addWidget(m_labelParameter);
    m_layout->addSpacerItem(new QSpacerItem(0,20,QSizePolicy::Expanding));
    m_layout->addWidget(m_labelValue);
    m_layout->addWidget(m_labelUnit);

    this->adjustSize();
}

//ReadParameterWidget::ReadParameterWidget(const Control &settings, int decimal, QString name = "Unknown", /*double value = 0,*/ QWidget *parent) :
//    QWidget(parent),
//    ui(new Ui::ReadParameterWidget),
//    m_settings(settings),
//    m_name(name)
//{
//    ui->setupUi(this);

//    ui->labelParameter->setText(m_name);

//    /*setValue(m_settings.real, value, decimal);*/

//}

//ReadParameterWidget::~ReadParameterWidget()
//{
//    delete ui;
//}

// private methods

void ReadParameterWidget::setUnit(QStringView unit){
    m_unit = unit.toString();
    m_labelUnit->setText(m_unit);
}

void ReadParameterWidget::setValue(double value, int decimal){
    QString realStr = QString::number(value, 'f', decimal);
    m_labelValue->setText(realStr);
}

void ReadParameterWidget::setValue(int value){
    m_labelValue->setNum(value);
}

int ReadParameterWidget::getUnitslength(){
    return m_labelUnit->text().length();
}

void ReadParameterWidget::setUnitsLength(int length){
    int charLength = 10;
    m_labelUnit->setMinimumWidth(length*charLength);
}
//testDevice
/*TestDeviceParameter::TestDeviceParameter(QObject *parent) : QObject(parent)
{
    Control* m_settings = new Control();

    m_settings->real =    0x0307;
    m_settings->name = "Current";
    settings.reset(m_settings);
    widget.reset(new ReadParameterWidget(*settings.get()));
}
TestDeviceParameter::~TestDeviceParameter()
{

}

void TestDeviceParameter::testLabel()
{
    QString units = "mA";
    int testDecimals = 5;
    double valueDouble = 15.5;
    int valueInt = 5;
    widget->setValue(settings->real, valueInt);
    widget->setUnits(units);
    QVERIFY(settings->name == widget->ui->labelParameter->text());
    QVERIFY(units == widget->ui->labelUnits->text());
    QVERIFY(QString::number(valueInt, 'f', 0) == widget->ui->labelValue->text());
    widget->setValue(settings->real, valueDouble, testDecimals);
    QVERIFY(QString::number(valueDouble, 'f', testDecimals) == widget->ui->labelValue->text());
}*/
