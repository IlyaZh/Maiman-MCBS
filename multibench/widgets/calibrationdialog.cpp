#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include "model/device/devicewidget.h"
#include "device/commandsettings.h"
#include "widgets/calibrationandlimitswidget.h"

const QString CalibrationDialog::styleButtonOn = "QPushButton {\
    color: rgb(255, 255, 255);\
background-color: rgb(51, 51, 51);\
border: 1px solid #999999;\
border-radius: 4px;\
padding: 4px;\
width: 50px\
}";

const QString CalibrationDialog::styleButtonOff = "QPushButton {\
    color: rgb(255, 255, 255);\
    background-color: rgb(153, 153, 153);\
    border: 1px solid #999999;\
    border-radius: 4px;\
    padding: 4px;\
    width: 50px\
}";

CalibrationDialog::CalibrationDialog(const DeviceWidgetDesc& deviceDesc, const QMap<quint16, QSharedPointer<DevCommand>>& commands, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog)
{
    ui->setupUi(this);
    QDialog::setWindowTitle("Calibrations And Limits");

    for (const auto& item : deviceDesc.calibration){
        auto calibrationWidget = new CalibrationAndLimitsWidget(item, commands.value(item.code));
        calibrationWidget->setParent(this);
        ui->calibrationLayout->addWidget(calibrationWidget);
        m_calibrationWidgets.append(calibrationWidget);
        connect(calibrationWidget, &CalibrationAndLimitsWidget::editFinished, this, &CalibrationDialog::widgetsAreValid);
    }

    for (const auto& item : deviceDesc.limits){
        auto limitWidget = new CalibrationAndLimitsWidget(item, commands.value(item.code), commands.value(item.maxCode), commands.value(item.minCode));
        limitWidget->setParent(this);
        ui->limitsLayout->addWidget(limitWidget);
        m_limitsWidgets.append(limitWidget);
        connect(limitWidget, &CalibrationAndLimitsWidget::editFinished, this, &CalibrationDialog::widgetsAreValid);
    }

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &CalibrationDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &CalibrationDialog::reject);
    connect(this, &CalibrationDialog::finished, this, [this](int result){
        switch (result) {
            case QDialog::Accepted:{
                saveResult();
            }break;
            case QDialog::Rejected:{
                deleteLater();
            }break;
        }
    });
}

CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}

void CalibrationDialog::saveResult(){
    for(const auto& item : qAsConst(m_calibrationWidgets )){
        item->sendValue();
    }
    for(const auto& item : qAsConst(m_limitsWidgets)) {
        item->sendValue();
    }
    this->deleteLater();
}

void CalibrationDialog::widgetsAreValid(){

    bool hasWrongWidgets = std::any_of(std::begin(m_calibrationWidgets),
                                       std::end(m_calibrationWidgets),
                                       [=](CalibrationAndLimitsWidget* item){
                                           return !item->getState();
                                       });
    bool hasWrongCalibrations = std::any_of(std::begin(m_limitsWidgets),
                                            std::end(m_limitsWidgets),
                                            [=](CalibrationAndLimitsWidget* item){
                                                return !item->getState();
                                            });
    bool state = !(hasWrongWidgets || hasWrongCalibrations);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(state);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setStyleSheet(state ? styleButtonOn : styleButtonOff);
}
