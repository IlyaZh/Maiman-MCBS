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

CalibrationAndLimitsWidget::CalibrationAndLimitsWidget(CalibrationKoef* calibration, QSharedPointer<DevCommand> command,QWidget *parent) :
    QDialog(parent),
    m_calibration(calibration),
    m_command(command),
    ui(new Ui::CalibrationAndLimitsWidget)
{
    ui->setupUi(this);
    maxValue = m_calibration->max;
    minValue = m_calibration->min;
    ui->nameParameter->setText(m_calibration->name);
    ui->value->setText(m_command->valueStr());
    m_validator = new QDoubleValidator(minValue,maxValue,m_command->tolerance());
    ui->maxParameter->setText(QString("Max:%1").arg(maxValue));
    ui->minParameter->setText(QString("Min:%1").arg(minValue));
    connect(ui->downValue,&QPushButton::clicked,this,&CalibrationAndLimitsWidget::decrement);
    connect(ui->upValue,&QPushButton::clicked,this,&CalibrationAndLimitsWidget::increment);

    connect(ui->value,&QLineEdit::editingFinished,this,&CalibrationAndLimitsWidget::finishedEdit);
    connect(ui->value,&QLineEdit::inputRejected,this,&CalibrationAndLimitsWidget::rejectedEdit);
}

CalibrationAndLimitsWidget::CalibrationAndLimitsWidget(Limit* limit,QSharedPointer<DevCommand> command,QSharedPointer<DevCommand> maxCommand,QSharedPointer<DevCommand> minCommand, QWidget *parent) :
    QDialog(parent),
    m_limit(limit),
    m_command(command),
    m_limitMaxCommand(maxCommand),
    m_limitMinCommand(minCommand),
    ui(new Ui::CalibrationAndLimitsWidget)
{
    ui->setupUi(this);
    ui->nameParameter->setText(m_limit->name);
    ui->value->setText(m_command->valueStr());

    if(!m_limitMaxCommand.isNull())
        maxValue = m_limitMaxCommand->valueDouble();
    else
        maxValue = m_limit->maxValue;
    ui->maxParameter->setText(QString("Max:%1").arg(maxValue));

    if(!m_limitMinCommand.isNull())
        minValue = m_limitMinCommand->valueDouble();
    else
        minValue = m_limit->minValue;
    ui->minParameter->setText(QString("Min:%1").arg(minValue));

    m_validator = new QDoubleValidator(minValue,maxValue,m_command->tolerance());
    connect(ui->downValue,&QPushButton::clicked,this,&CalibrationAndLimitsWidget::decrement);
    connect(ui->upValue,&QPushButton::clicked,this,&CalibrationAndLimitsWidget::increment);

    connect(ui->value,&QLineEdit::editingFinished,this,&CalibrationAndLimitsWidget::finishedEdit);
    connect(ui->value,&QLineEdit::inputRejected,this,&CalibrationAndLimitsWidget::rejectedEdit);

}

CalibrationAndLimitsWidget::~CalibrationAndLimitsWidget()
{
    delete ui;
}

void CalibrationAndLimitsWidget::increment(){
    double value = ui->value->text().toDouble();
    value = value + 1.0/m_command->divider();
    ui->value->setText(QString::number(value,'f',m_command->tolerance()));
    finishedEdit();
}

void CalibrationAndLimitsWidget::decrement(){
    double value = ui->value->text().toDouble();
    value = value - 1.0/m_command->divider();
    ui->value->setText(QString::number(value,'f',m_command->tolerance()));
    finishedEdit();
}

void CalibrationAndLimitsWidget::sendValue(){
    if(!m_command.isNull())
        m_command->setFromWidget(ui->value->text().toDouble());
}

void CalibrationAndLimitsWidget::finishedEdit(){
    QString v=ui->value->text();
    int pos = 0;
    if(m_validator->validate(v,pos) == QValidator::Acceptable){
        ui->value->setStyleSheet(styleSheetOK);
        double value = ui->value->text().toDouble();
        ui->value->setText(QString::number(value,'f',m_command->tolerance()));
        m_state = true;
    }
    else{
        ui->value->setStyleSheet(styleSheetERROR);
        m_state = false;
    }
    emit checkValueRange(m_state);
}
void CalibrationAndLimitsWidget::rejectedEdit(){
    qDebug()<<"INPUT REJECTED";
    ui->value->clear();
    ui->value->setText(m_command->valueStr());
}
