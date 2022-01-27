#include "plusminuswidget.h"
#include "ui_calibrationandlimitswidget.h"
#include "model/device/devicewidget.h"
#include "device/commandsettings.h"

const QString PlusMinusWidget::styleSheetOK {"\
    QLineEdit {\
        font: 16pt Share Tech Mono;\
        color: rgb(16, 33, 40);\
        background: rgb(255, 255, 255);\
        border-radius: 5px;\
        padding: 5px 0;\
    }\
    QLineEdit::disabled {\
        background: rgb(255, 255, 255);\
        color: rgb(76, 93, 100);\
        border-radius: 5px;\
        padding: 5px 0;\
    }"};

const QString PlusMinusWidget::styleSheetERROR {"\
    QLineEdit {\
        font: 16pt Share Tech Mono;\
        color: rgb(255, 0, 0);\
        background: rgb(255, 255, 255);\
        border-radius: 5px;\
        padding: 5px 0;\
}\
    QLineEdit::disabled {\
        background: rgb(255, 255, 255);\
        color: rgb(255, 0, 0);\
        border-radius: 5px;\
        padding: 5px 0;\
}"};

PlusMinusWidget::PlusMinusWidget(const CalibrationKoef& calibration, QSharedPointer<DevCommand> command, QWidget *parent) :
    QDialog(parent),
    m_command(command),
    ui(new Ui::CalibrationAndLimitsWidget),
    m_validator(new QDoubleValidator(this))
{
    ui->setupUi(this);
    ui->nameParameter->setText(calibration.name);
    ui->value->setText(m_command->valueStr());
    m_validator->setDecimals(m_command->tolerance());

    setMax(calibration.max);
    setMin(calibration.min);

    delta = 1.0/m_command->divider();

    connect(ui->downValue,&QPushButton::clicked,this,&PlusMinusWidget::decrement);
    connect(ui->upValue,&QPushButton::clicked,this,&PlusMinusWidget::increment);

    connect(ui->value,&QLineEdit::textChanged,this,&PlusMinusWidget::editedValue);
    connect(ui->value,&QLineEdit::inputRejected,this,&PlusMinusWidget::rejectedEdit);
    connect(ui->value,&QLineEdit::editingFinished,this,&PlusMinusWidget::inputCompleted);
}

PlusMinusWidget::PlusMinusWidget(const Limit& limit, QSharedPointer<DevCommand> command, QSharedPointer<DevCommand> maxCommand, QSharedPointer<DevCommand> minCommand, QWidget *parent) :
    QDialog(parent),
    m_command(command),
    ui(new Ui::CalibrationAndLimitsWidget),
    m_validator(new QDoubleValidator(this))
{
    ui->setupUi(this);
    ui->nameParameter->setText(limit.name);
    ui->value->setText(m_command->valueStr());
    m_validator->setDecimals(m_command->tolerance());

    setMax((maxCommand.isNull()) ? limit.maxValue : maxCommand->valueDouble());
    setMin((minCommand.isNull()) ? limit.minValue : minCommand->valueDouble());

    delta = 1.0/m_command->divider();

    connect(ui->downValue,&QPushButton::clicked,this,&PlusMinusWidget::decrement);
    connect(ui->upValue,&QPushButton::clicked,this,&PlusMinusWidget::increment);

    connect(ui->value,&QLineEdit::textChanged,this,&PlusMinusWidget::editedValue);
    connect(ui->value,&QLineEdit::inputRejected,this,&PlusMinusWidget::rejectedEdit);
    connect(ui->value,&QLineEdit::editingFinished,this,&PlusMinusWidget::inputCompleted);
}

PlusMinusWidget::~PlusMinusWidget()
{
    delete ui;
}

void PlusMinusWidget::increment(){
    double value = ui->value->text().toDouble();
    value += delta;
    value = qBound(minValue, value, maxValue);
    ui->value->setText(QString::number(value,'f',m_command->tolerance()));
}

void PlusMinusWidget::decrement(){
    double value = ui->value->text().toDouble();
    value -= delta;
    value = qBound(minValue, value, maxValue);
    ui->value->setText(QString::number(value,'f',m_command->tolerance()));
}

void PlusMinusWidget::sendValue(){
    if(!m_command.isNull())
        m_command->setFromWidget(ui->value->text().toDouble());
}

double PlusMinusWidget::value() {
    return ui->value->text().toDouble();
}

void PlusMinusWidget::setMin(double min) {
    minValue = min;
    m_validator->setBottom(minValue);
    ui->minParameter->setText(QString("Min:%1").arg(minValue));
}

void PlusMinusWidget::setMax(double max) {
    maxValue = max;
    m_validator->setTop(maxValue);
    ui->maxParameter->setText(QString("Max:%1").arg(maxValue));
}

void PlusMinusWidget::editedValue(){
    QString v=ui->value->text();
    int pos = 0;
    if(m_validator->validate(v,pos) == QValidator::Acceptable){
        ui->value->setStyleSheet(styleSheetOK);
        m_state = true;
    }
    else{
        ui->value->setStyleSheet(styleSheetERROR);
        m_state = false;
    }
    emit editFinished();
}

void PlusMinusWidget::inputCompleted(){
    double value = ui->value->text().toDouble();
    ui->value->setText(QString::number(value,'f',m_command->tolerance()));
}

void PlusMinusWidget::rejectedEdit(){
    ui->value->clear();
    ui->value->setText(m_command->valueStr());
}

bool PlusMinusWidget::state(){
    return m_state;
}
