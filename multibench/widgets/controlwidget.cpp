#include "controlwidget.h"
#include "ui_controlwidget.h"
#include "model/device/devicewidget.h"
#include <QDebug>

enum {ErrorTimeout=1000};

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
        m_Validator = new QDoubleValidator(m_Min->valueDouble(),m_Max->valueDouble(),m_Value->tolerance());

        //ui->Value->setValidator(m_Validator);
        setMinValue();
        setMaxValue();
        setValue();
        if (m_Real.isNull()){
            ui->Real->setText("Set");
            setRealValue();
        }
        else{
            ui->Real->setText("Real");
            setRealValue(/*m_Real->valueDouble(),m_Real->tolerance()*/);
        }
        setUnits(m_Value->unit());

        connect(m_Value.get(), &DevCommand::updatedValue, this, &ControlWidget::setValue);

        connect(m_Value.get(), &DevCommand::updatedUnit, this, &ControlWidget::setUnits);
        setUnits(m_Value->unit());
        connect(m_Min.get(), &DevCommand::updatedValue, this, &ControlWidget::setMinValue);
        connect(m_Max.get(), &DevCommand::updatedValue, this, &ControlWidget::setMaxValue);
    }

    if(m_Real) {
        connect(m_Real.get(), &DevCommand::updatedValue, this, &ControlWidget::setRealValue);
    }

    connect(ui->Value, &QLineEdit::returnPressed,
            this, &ControlWidget::userEnteredValue);
    connect(ui->Value, &QLineEdit::cursorPositionChanged,
            this, [this](){
                isUserEdit = true;
            });
    //connect(ui->Value, &QLineEdit::textChanged, this, &ControlWidget::setEditLineRed);
    adjust();
}

ControlWidget::~ControlWidget()
{
    delete ui;
}

void ControlWidget::userEnteredValue(){
    double valueFromLine =  ui->Value->text().toDouble();
    ui->Value->clearFocus();

    if(valueFromLine > m_Max->valueDouble()){
        valueFromLine = m_Max->valueDouble();
        setEditLineRed();
        QTimer::singleShot(ErrorTimeout, this, SLOT(setEditLineWhite()));
    }
    else if(valueFromLine < m_Min->valueDouble()){
        valueFromLine = m_Min->valueDouble();
        setEditLineRed();
        QTimer::singleShot(ErrorTimeout, this, SLOT(setEditLineWhite()));
    }
    qDebug() << ui->Value->text();

    if(!m_Value.isNull()) {
        m_Value->setFromWidget(valueFromLine);
    }

    qDebug() << "SIGNAL CommandWidget" << m_Value->code() << valueFromLine << m_Value->valueStr();

//    if(valueFromLine >= m_Min->valueDouble() && valueFromLine <= m_Max->valueDouble()) {
//        qDebug() << ui->Value->text();

//        if(!m_Value.isNull()) {
//            m_Value->setFromWidget(valueFromLine);
//        }

//        qDebug() << "SIGNAL CommandWidget" << m_Value->code() << valueFromLine << m_Value->valueStr();
//    }
    isUserEdit = false;
}

void ControlWidget::setValue(){
    if (m_Value->code() == 0x07) qDebug()<<"SET VALUE ______________________________________________________________________________"<<m_Value->valueInt()<<isUserEdit;
    const QString value = m_Value->valueStr();
    if(!isUserEdit) {
        if(m_Value) {
            ui->Value->setText(value);
        }
    }
    if(m_Real.isNull()) {
        ui->RealValue->setText(value);
    }
}


void ControlWidget::setUnits(QStringView units){
    ui->RealUnits->setText(units.toString());
    ui->MaxUnits->setText(units.toString());
    ui->MinUnits->setText(units.toString());
}

void ControlWidget::setMaxValue(){
    ui->MaxValue->setText(m_Max->valueStr());
    m_Validator->setTop(m_Max->valueDouble());
}

void ControlWidget::setMinValue(){
    ui->MinValue->setText(m_Min->valueStr());
    m_Validator->setBottom(m_Min->valueDouble());
}

void ControlWidget::setRealValue(){
    if(m_Real)
        ui->RealValue->setText(m_Real->valueStr());
}

// private method
void ControlWidget::adjust() {
    this->adjustSize();
    this->setMinimumSize(this->size());
}

void ControlWidget::setEditLineRed(){
    ui->Value->setStyleSheet("  color: rgb(255, 255, 255);\n"
                             "	background: rgb(230, 0, 0);\n"
                             "	border-radius: 5px;\n");

}

void ControlWidget::setEditLineWhite(){
    ui->Value->setStyleSheet("  color: rgb(0, 0, 0);\n"
                             "	background: rgb(255, 255, 255);\n"
                             "	border-radius: 5px;\n");
    const QString value = m_Value->valueStr();
    ui->Value->setText(value);
}
