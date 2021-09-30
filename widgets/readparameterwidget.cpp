#include "readparameterwidget.h"
#include "ui_readparameterwidget.h"
#include "model/device/devicewidget.h"
#include "device/commandsettings.h"

// TODO: почисти хлам

ReadParameterWidget::ReadParameterWidget(const Control &settings, QSharedPointer<DevCommand> cmd, /*int value, QString name,*/ QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReadParameterWidget),
    m_settings(settings),
    m_command(cmd)
{
    ui->setupUi(this);

    ui->labelParameter->setText(m_settings.name);

    ui->labelUnits->setText(m_command->unit());

    /*setValue(m_settings.real, value);*/

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


void ReadParameterWidget::setUnits(const QString& unit){
    m_unit = unit;
    ui->labelUnits->setText(unit);
}

void ReadParameterWidget::setValue(quint16 cmd, double value, int decimal){
    QString realStr = QString::number(value, 'f', decimal);
    if (cmd == m_settings.real){
        ui->labelValue->setText(realStr);
        m_values.insert(cmd, value);
        m_decimals.insert(cmd, decimal);
    }
}

void ReadParameterWidget::setValue(quint16 cmd, int value){
    if (cmd == m_settings.real){
        ui->labelValue->setNum(value);
        m_values.insert(cmd, value);
        m_decimals.insert(cmd, 0);
    }
}

void ReadParameterWidget::setDecimals(quint16 cmd, int decimals){
    if (cmd == m_settings.real){
        m_decimals.insert(cmd, decimals);
    }
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
