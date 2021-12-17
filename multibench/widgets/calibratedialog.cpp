#include "calibratedialog.h"
#include "ui_calibratedialog.h"
#include "model/device/devicewidget.h"
#include "device/commandsettings.h"
#include "widgets/calibrationandlimitswidget.h"

const QString CalibrateDialog::styleButtonOn = "QPushButton {\
    color: rgb(255, 255, 255);\
background-color: rgb(51, 51, 51);\
border: 1px solid #999999;\
border-radius: 4px;\
padding: 4px;\
width: 50px\
}";

const QString CalibrateDialog::styleButtonOff = "QPushButton {\
    color: rgb(255, 255, 255);\
    background-color: rgb(153, 153, 153);\
    border: 1px solid #999999;\
    border-radius: 4px;\
    padding: 4px;\
    width: 50px\
}";

CalibrateDialog::CalibrateDialog(QVector<CalibrationAndLimitsWidget*> calibrations,QVector<CalibrationAndLimitsWidget*> limits, QWidget *parent) :
    QDialog(parent),
    m_calibrationWidgets(calibrations),
    m_limitsWidgets(limits),
    ui(new Ui::CalibrateDialog)
{
    ui->setupUi(this);
    QDialog::setWindowTitle("Calibrations And Limits");
    for(int i=0;i<m_calibrationWidgets.size();i++){
        ui->calibrationLayout->addWidget(m_calibrationWidgets.at(i));
        connect(m_calibrationWidgets.at(i),&CalibrationAndLimitsWidget::checkValueRange,this,&CalibrateDialog::setButtonState);
    }
    for(int i=0;i<m_limitsWidgets.size();i++){
        ui->limitsLayout->addWidget(m_limitsWidgets.at(i));
        connect(m_limitsWidgets.at(i),&CalibrationAndLimitsWidget::checkValueRange,this,&CalibrateDialog::setButtonState);
    }
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveResult()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(deleteLater()));
}

CalibrateDialog::~CalibrateDialog()
{
    delete ui;
}

void CalibrateDialog::saveResult(){
    for(int i=0;i<m_calibrationWidgets.size();i++){
        m_calibrationWidgets.at(i)->sendValue();
    }
    for(int i=0;i<m_limitsWidgets.size();i++){
        m_limitsWidgets.at(i)->sendValue();
    }
    this->deleteLater();
}

void CalibrateDialog::setButtonState(bool state){
    for(int i=0;i<m_limitsWidgets.size();i++){
        state = state & m_limitsWidgets.at(i)->m_state;
    }
    for(int i=0;i<m_calibrationWidgets.size();i++){
        state = state & m_calibrationWidgets.at(i)->m_state;
    }
    if(state){
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setStyleSheet(styleButtonOn);
    }
    else{
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setStyleSheet(styleButtonOff);
    }
}
