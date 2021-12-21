#include "calibrationmenu.h"
#include "ui_calibrationmenu.h"
#include "model/device/devicewidget.h"
#include "device/commandsettings.h"
#include "widgets/calibrationandlimitswidget.h"

const QString CalibrationMenu::styleButtonOn = "QPushButton {\
    color: rgb(255, 255, 255);\
background-color: rgb(51, 51, 51);\
border: 1px solid #999999;\
border-radius: 4px;\
padding: 4px;\
width: 50px\
}";

const QString CalibrationMenu::styleButtonOff = "QPushButton {\
    color: rgb(255, 255, 255);\
    background-color: rgb(153, 153, 153);\
    border: 1px solid #999999;\
    border-radius: 4px;\
    padding: 4px;\
    width: 50px\
}";

CalibrationMenu::CalibrationMenu(quint16 id,QMap<quint16, DeviceWidgetDesc> deviceDesc, const QMap<quint16, QSharedPointer<DevCommand>>& commands, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationMenu),
    m_id(id),
    m_commands(commands),
    m_deviceWidgets(deviceDesc)
{
    ui->setupUi(this);
    QDialog::setWindowTitle("Calibrations And Limits");

    for (auto& item:m_deviceWidgets[id].calibration){
        auto calibrationWidget = new CalibrationAndLimitsWidget(&item, commands.value(item.code));
        ui->calibrationLayout->addWidget(calibrationWidget);
        m_calibrationWidgets.append(calibrationWidget);
        connect(calibrationWidget,&CalibrationAndLimitsWidget::editFinished,this,&CalibrationMenu::widgetsAreValid);
    }

    for (auto& item:m_deviceWidgets[id].limits){
        auto limitWidget = new CalibrationAndLimitsWidget(&item, commands.value(item.code),commands.value(item.maxCode),commands.value(item.minCode));
        ui->limitsLayout->addWidget(limitWidget);
        m_limitsWidgets.append(limitWidget);
        connect(limitWidget,&CalibrationAndLimitsWidget::editFinished,this,&CalibrationMenu::widgetsAreValid);
    }

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveResult()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(deleteLater()));
}

CalibrationMenu::~CalibrationMenu()
{
    delete ui;
}

void CalibrationMenu::saveResult(){
    for(auto item:m_calibrationWidgets){
        item->sendValue();
    }
    for(auto item:m_limitsWidgets){
        item->sendValue();
    }
    this->deleteLater();
}

void CalibrationMenu::widgetsAreValid(){

    bool hasWrongWidgets = std::any_of(std::begin(m_calibrationWidgets),std::end(m_calibrationWidgets),[=](CalibrationAndLimitsWidget* item){return !item->getState();});
    bool hasWrongCalibrations = std::any_of(std::begin(m_limitsWidgets),std::end(m_limitsWidgets),[=](CalibrationAndLimitsWidget* item){return !item->getState();});
    bool state = !(hasWrongWidgets || hasWrongCalibrations);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(state);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setStyleSheet(state ? styleButtonOn : styleButtonOff);
}
