#include "calibrationandlimitswidget.h"
#include "ui_calibrationandlimitswidget.h"
#include "model/device/devicewidget.h"
#include "device/commandsettings.h"

const QString CalibrationAndLimitsWidget::nameStyle ="color: rgb(255, 255, 255);";
const QString CalibrationAndLimitsWidget::minorParametersStyle = "color: rgb(230,230,230);";
const QString CalibrationAndLimitsWidget::buttonsStyle = "QPushButton {\
                                background: rgb(16, 33, 40);\
                                color: rgb(230, 230, 230);\
                                border-radius: 5px;\
                                width: 20px;\
                                height: 30px;\
                                }\
                                QPushButton::disabled {\
                                background: rgb(76, 93, 100);\
                                color: rgb(180, 180, 180);}";
const QString CalibrationAndLimitsWidget::lineEditStyle="QLineEdit {\
                                color: rgb(16, 33, 40);\
                                background: rgb(230, 230, 230);\
                                border-radius: 5px;\
                                padding: 5px 0;\
                                }\
                                QLineEdit::disabled {\
                                background: rgb(180, 180, 180);\
                                color: rgb(76, 93, 100);}";

CalibrationAndLimitsWidget::CalibrationAndLimitsWidget(CalibrationKoef* calibration, QSharedPointer<DevCommand> command,QWidget *parent) :
    QDialog(parent),
    m_calibration(calibration),
    m_command(command),
    ui(new Ui::CalibrationAndLimitsWidget)
{
    ui->setupUi(this);
    ui->nameParameter->setText(m_calibration->name);
    ui->value->setText(m_command->valueStr());
    ui->maxParameter->setText(QString("Max:%1").arg(m_calibration->max));
    ui->minParameter->setText(QString("Min:%1").arg(m_calibration->min));
    connect(ui->downValue,&QPushButton::clicked,this,&CalibrationAndLimitsWidget::decrement);
    connect(ui->upValue,&QPushButton::clicked,this,&CalibrationAndLimitsWidget::increment);
    //connect(m_command.get(),&DevCommand::updatedValue,this,[=](){ui->value->setText(m_command->valueStr());});
}

CalibrationAndLimitsWidget::CalibrationAndLimitsWidget(Limit* limit,QSharedPointer<DevCommand> command,QSharedPointer<DevCommand> limitCommand, QWidget *parent) :
    QDialog(parent),
    m_limit(limit),
    m_command(command),
    m_limitCommand(limitCommand),
    ui(new Ui::CalibrationAndLimitsWidget)
{
    ui->setupUi(this);
    ui->nameParameter->setText(m_limit->name);
    ui->value->setText(QString::number(qRound(m_command->valueDouble())));

    connect(ui->downValue,&QPushButton::clicked,this,&CalibrationAndLimitsWidget::decrement);
    connect(ui->upValue,&QPushButton::clicked,this,&CalibrationAndLimitsWidget::increment);

}

CalibrationAndLimitsWidget::~CalibrationAndLimitsWidget()
{
    delete ui;
}

void CalibrationAndLimitsWidget::increment(){
    double value = ui->value->text().toDouble();
    value = value + 1.0/m_command->divider();
    ui->value->setText(QString::number(value));
}

void CalibrationAndLimitsWidget::decrement(){
    double value = ui->value->text().toDouble();
    value = value - 1.0/m_command->divider();
    ui->value->setText(QString::number(value));
}

void CalibrationAndLimitsWidget::sendValue(){
    if(!m_command.isNull())
        m_command->setFromWidget(ui->value->text().toDouble()/m_command->divider());
}
