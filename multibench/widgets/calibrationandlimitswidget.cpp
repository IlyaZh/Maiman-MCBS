#include "calibrationandlimitswidget.h"
#include "ui_calibrationandlimitswidget.h"
#include "model/device/devicewidget.h"
#include "device/commandsettings.h"

const QString CalibrationAndLimitsWidget::styleSheetOK = "\
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
}";

const QString CalibrationAndLimitsWidget::styleSheetERROR = "\
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
}";

CalibrationAndLimitsWidget::CalibrationAndLimitsWidget(const CalibrationKoef& calibration, QSharedPointer<DevCommand> command,QWidget *parent) :
    QDialog(parent),
    m_command(command),
    ui(new Ui::CalibrationAndLimitsWidget)
    //m_calibration(calibration)
{
    ui->setupUi(this);
    maxValue = calibration.max;
    minValue = calibration.min;
    ui->nameParameter->setText(calibration.name);
    ui->value->setText(m_command->valueStr());
    m_validator = new QDoubleValidator(minValue,maxValue,m_command->tolerance());
    ui->maxParameter->setText(QString("Max:%1").arg(maxValue));
    ui->minParameter->setText(QString("Min:%1").arg(minValue));

    delta = 1.0/m_command->divider();

    connect(ui->downValue,&QPushButton::clicked,this,&CalibrationAndLimitsWidget::decrement);
    connect(ui->upValue,&QPushButton::clicked,this,&CalibrationAndLimitsWidget::increment);

    connect(ui->value,&QLineEdit::textChanged,this,&CalibrationAndLimitsWidget::editedValue);
    connect(ui->value,&QLineEdit::inputRejected,this,&CalibrationAndLimitsWidget::rejectedEdit);
    connect(ui->value,&QLineEdit::editingFinished,this,&CalibrationAndLimitsWidget::inputCompleted);
}

CalibrationAndLimitsWidget::CalibrationAndLimitsWidget(const Limit& limit,QSharedPointer<DevCommand> command,QSharedPointer<DevCommand> maxCommand,QSharedPointer<DevCommand> minCommand, QWidget *parent) :
    QDialog(parent),
    m_command(command),
    ui(new Ui::CalibrationAndLimitsWidget)
    //m_limit(limit)
{
    ui->setupUi(this);
    ui->nameParameter->setText(limit.name);
    ui->value->setText(m_command->valueStr());

    if(!maxCommand.isNull())
        maxValue = maxCommand->valueDouble();
    else
        maxValue = limit.maxValue;
    ui->maxParameter->setText(QString("Max:%1").arg(maxValue));

    if(!minCommand.isNull())
        minValue = minCommand->valueDouble();
    else
        minValue = limit.minValue;
    ui->minParameter->setText(QString("Min:%1").arg(minValue));

    delta = 1.0/m_command->divider();

    m_validator = new QDoubleValidator(minValue,maxValue,m_command->tolerance());
    connect(ui->downValue,&QPushButton::clicked,this,&CalibrationAndLimitsWidget::decrement);
    connect(ui->upValue,&QPushButton::clicked,this,&CalibrationAndLimitsWidget::increment);

    connect(ui->value,&QLineEdit::textChanged,this,&CalibrationAndLimitsWidget::editedValue);
    connect(ui->value,&QLineEdit::inputRejected,this,&CalibrationAndLimitsWidget::rejectedEdit);
    connect(ui->value,&QLineEdit::editingFinished,this,&CalibrationAndLimitsWidget::inputCompleted);
}

CalibrationAndLimitsWidget::~CalibrationAndLimitsWidget()
{
    delete ui;
}

void CalibrationAndLimitsWidget::increment(){
    double value = ui->value->text().toDouble();
    value += delta;
    value = qBound(minValue, value, maxValue);
    ui->value->setText(QString::number(value,'f',m_command->tolerance()));
}

void CalibrationAndLimitsWidget::decrement(){
    double value = ui->value->text().toDouble();
    value -= delta;
    value = qBound(minValue, value, maxValue);
    ui->value->setText(QString::number(value,'f',m_command->tolerance()));
}

void CalibrationAndLimitsWidget::sendValue(){
    if(!m_command.isNull())
        m_command->setFromWidget(ui->value->text().toDouble());
}

void CalibrationAndLimitsWidget::editedValue(){
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

void CalibrationAndLimitsWidget::inputCompleted(){
    double value = ui->value->text().toDouble();
    ui->value->setText(QString::number(value,'f',m_command->tolerance()));
}

void CalibrationAndLimitsWidget::rejectedEdit(){
    ui->value->clear();
    ui->value->setText(m_command->valueStr());
}

bool CalibrationAndLimitsWidget::getState(){
    return m_state;
}
