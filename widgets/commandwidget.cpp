#include "commandwidget.h"
#include "ui_commandwidget.h"
#include "model/device/devicewidget.h"

CommandWidget::CommandWidget(const Control& settings,
                             QSharedPointer<DevCommand> Value,
                             QSharedPointer<DevCommand> Max,
                             QSharedPointer<DevCommand> Min,
                             QSharedPointer<DevCommand> Real,
                             QWidget *parent):
    QWidget(parent),
    ui(new Ui::CommandWidget),
    m_settings(settings),
    m_Real(Real),
    m_Value(Value),
    m_Max(Max),
    m_Min(Min)
{
    ui->setupUi(this);

    ui->WidgetName->setText(m_settings.name);

    if (m_Value != nullptr and m_Max != nullptr and m_Min != nullptr){
        m_Validator.setRange(m_Min->valueDouble(),m_Max->valueDouble(),m_Value->tolerance());

        ui->Value->setValidator(&m_Validator);
        setMinValue(m_Min->valueDouble(),m_Min->tolerance());
        setMaxValue(m_Max->valueDouble(),m_Max->tolerance());
        setValue(m_Value->valueDouble(),m_Value->tolerance());
        if (m_Real == nullptr){
            ui->Real->setText("Set");
            setRealValue(m_Value->valueDouble(),m_Value->tolerance());
        }
        else{
            ui->Real->setText("Real");
            setRealValue(m_Real->valueDouble(),m_Real->tolerance());
        }
        setUnits(m_Value->unit());
    }
    connect(ui->Value, &QLineEdit::editingFinished, this, &CommandWidget::LineEdit_EditFinished);

    adjust();
    qDebug() << "CommandWIdget size=" << this->size();

}

CommandWidget::~CommandWidget()
{
    delete ui;
}

void CommandWidget::LineEdit_EditFinished(){

    double valueFromLine;
//    if (m_Real == nullptr)
//        ui->RealValue->setText(ui->Value->text());

    valueFromLine =  ui->Value->text().toDouble();
    emit valueChanged(m_Value->code(), valueFromLine);
    qDebug() << "SIGNAL CommandWidget" << m_Value->code() << valueFromLine;
}

void CommandWidget::setValue(double value, int decimal){
    QString valueStr = QString::number(value, 'f', decimal);
    ui->RealValue->setText(valueStr);
    ui->Value->setText(valueStr);
}

void CommandWidget::setUnits(QString units){
    ui->RealUnits->setText(units);
    ui->MaxUnits->setText(units);
    ui->MinUnits->setText(units);
}

void CommandWidget::setMaxValue(double value, int decimal){
    QString valueStr = QString::number(value, 'f', decimal);
    ui->MaxValue->setText(valueStr);
}

void CommandWidget::setMinValue(double value, int decimal){
    QString valueStr = QString::number(value, 'f', decimal);
    ui->MinValue->setText(valueStr);
}

void CommandWidget::setRealValue(double value, int decimal){
    QString valueStr = QString::number(value, 'f', decimal);
    ui->RealValue->setText(valueStr);
}

// private method
void CommandWidget::adjust() {
    this->adjustSize();
    this->setMinimumSize(this->size());
}
