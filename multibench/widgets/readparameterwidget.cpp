#include "readparameterwidget.h"
#include "ui_readparameterwidget.h"
#include "model/device/devicewidget.h"
#include "device/commandsettings.h"

ReadParameterWidget::ReadParameterWidget(QStringView name, QSharedPointer<DevCommand> cmd, /*int value, QString name,*/ QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReadParameterWidget),
//    m_settings(settings),
    m_command(cmd)
{
    ui->setupUi(this);

    ui->labelParameter->setText(name.toString());

    ui->labelUnits->setText(m_command->unit());

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
                ui->labelValue->setText("");
            else
                setUnit(unit.toString());
        });
    }

    /*setValue(m_settings.real, value);*/
    setValue(m_command->valueDouble(), m_command->tolerance());
    if (m_command->code() == 3)
        ui->labelValue->setText("");
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

ReadParameterWidget::~ReadParameterWidget()
{
    delete ui;
}

// private methods

void ReadParameterWidget::setUnit(QStringView unit){
    m_unit = unit.toString();
    ui->labelUnits->setText(m_unit);
}

void ReadParameterWidget::setValue(double value, int decimal){
    QString realStr = QString::number(value, 'f', decimal);
    ui->labelValue->setText(realStr);
}

void ReadParameterWidget::setValue(int value){
    ui->labelValue->setNum(value);
}

int ReadParameterWidget::getUnitslength(){
    return ui->labelUnits->text().length();
}

void ReadParameterWidget::setUnitsLength(int length){
    int charLength = 10;
    ui->labelUnits->setMinimumWidth(length*charLength);
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
