#include "calibratedialog.h"
#include "ui_calibratedialog.h"
#include "model/device/devicewidget.h"
#include "device/commandsettings.h"
#include "widgets/calibrationandlimitswidget.h"

CalibrateDialog::CalibrateDialog(QVector<CalibrationAndLimitsWidget*> calibrations,QVector<CalibrationAndLimitsWidget*> limits, QWidget *parent) :
    QDialog(parent),
    m_calibrationWidgets(calibrations),
    m_limitsWidgets(limits),
    ui(new Ui::CalibrateDialog)
{
    ui->setupUi(this);
    for(int i=0;i<m_calibrationWidgets.size();i++){
        ui->calibrationLayout->addWidget(m_calibrationWidgets.at(i));
    }
    for(int i=0;i<m_limitsWidgets.size();i++){
        ui->limitsLayout->addWidget(m_limitsWidgets.at(i));
    }
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveResult()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(hide()));
}

CalibrateDialog::~CalibrateDialog()
{
    delete ui;
}

void CalibrateDialog::saveResult(){
    for(int i=0;i<m_calibrationWidgets.size();i++){
        m_calibrationWidgets.at(i)->sendValue();
    }
    this->hide();
}
