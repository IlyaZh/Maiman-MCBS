#include "controlwidget.h"
#include "ui_controlwidget.h"
#include "model/device/devicewidget.h"
#include <QDebug>

ControlWidget::ControlWidget(QStringView name,
                             QSharedPointer<DevCommand> Value,
                             QSharedPointer<DevCommand> Max,
                             QSharedPointer<DevCommand> Min,
                             QSharedPointer<DevCommand> Real,
                             QWidget *parent):
    QWidget(parent),
    ui(new Ui::CommandWidget),
    m_Real(Real),
    m_Value(Value),
    m_Max(Max),
    m_Min(Min)
{
    ui->setupUi(this);

    ui->WidgetName->setText(name.toString());

    if (m_Value and m_Max and m_Min){
//        m_Validator = new QDoubleValidator(this);
//        m_Validator->setRange(m_Min->valueDouble(),m_Max->valueDouble(),m_Value->tolerance());

//        ui->Value->setValidator(m_Validator);
        setMinValue(m_Min->valueDouble(),m_Min->tolerance());
        setMaxValue(m_Max->valueDouble(),m_Max->tolerance());
        setValue(/*m_Value->valueDouble(),m_Value->tolerance()*/);
        if (m_Real.isNull()){
            ui->Real->setText("Set");
            setRealValue(m_Value->valueDouble(),m_Value->tolerance());
        }
        else{
            ui->Real->setText("Real");
            setRealValue(m_Real->valueDouble(),m_Real->tolerance());
        }
        setUnits(m_Value->unit());

        connect(m_Value.get(), &DevCommand::updatedValue, this, [this](){
            if(!isUserEdit) {
                setValue(/*m_Value->valueDouble(), m_Value->tolerance()*/);
            }
        });
        connect(m_Min.get(), &DevCommand::updatedValue, this, [this](){
            setMinValue(m_Min->valueDouble(), m_Min->tolerance());
        });
        connect(m_Max.get(), &DevCommand::updatedValue, this, [this](){
            setMaxValue(m_Max->valueDouble(), m_Max->tolerance());
        });
    }

    if(m_Real) {
        connect(m_Real.get(), &DevCommand::updatedValue, this, [this](){
            setRealValue(m_Real->valueDouble(), m_Real->tolerance());
        });
    }

//    connect(ui->Value, &QLineEdit::editingFinished,
    connect(ui->Value, &QLineEdit::returnPressed,
            this, &ControlWidget::userEnteredValue);
    connect(ui->Value, &QLineEdit::cursorPositionChanged,
            this, [this](){
                isUserEdit = true;
            });
//    connect(ui->Value, &QLineEdit::);

    adjust();
}

ControlWidget::~ControlWidget()
{
    delete ui;
}

void ControlWidget::userEnteredValue(){
    double valueFromLine =  ui->Value->text().toDouble();

    if(valueFromLine >= m_Min->valueDouble() && valueFromLine <= m_Max->valueDouble()) {
        qDebug() << ui->Value->text();

        if(!m_Value.isNull()) {
            m_Value->setFromWidget(valueFromLine);
        }

        qDebug() << "SIGNAL CommandWidget" << m_Value->code() << valueFromLine << m_Value->valueStr();
//        ui->Value->setText(m_Value->valueStr());
    }
    isUserEdit = false;
}

void ControlWidget::setValue(/*double value, int decimal*/){
    if(!isUserEdit) {
        if(m_Value) {
            const QString value = m_Value->valueStr();
            ui->Value->setText(value);

            if(m_Real.isNull()) {
                ui->RealValue->setText(value);
            }
        }
    }
}


void ControlWidget::setUnits(QString units){
    ui->RealUnits->setText(units);
    ui->MaxUnits->setText(units);
    ui->MinUnits->setText(units);
}

void ControlWidget::setMaxValue(double value, int decimal){
//    QString maxStr = QString::number(value, 'f', decimal);
//    ui->MaxValue->setText(maxStr);
    ui->MaxValue->setText(m_Max->valueStr());
}

void ControlWidget::setMinValue(double value, int decimal){
//    QString minStr = QString::number(value, 'f', decimal);
//    ui->MinValue->setText(minStr);
    ui->MinValue->setText(m_Min->valueStr());
}

void ControlWidget::setRealValue(double value, int decimal){
//    QString realStr = QString::number(value, 'f', decimal);
//    ui->RealValue->setText(realStr);
    if(m_Real)
        ui->RealValue->setText(m_Real->valueStr());
}

// private method
void ControlWidget::adjust() {
    this->adjustSize();
    this->setMinimumSize(this->size());
}
