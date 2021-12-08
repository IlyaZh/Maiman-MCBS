#include "calibratedialog.h"
#include "ui_calibratedialog.h"
#include "model/device/devicewidget.h"
#include "device/commandsettings.h"

CalibrateDialog::CalibrateDialog(CalibrationKoef* calibration,QSharedPointer<DevCommand> command, QWidget *parent) :
    QDialog(parent),
    m_calibraion(calibration),
    m_command(command),
    ui(new Ui::CalibrateDialog)
{
    ui->setupUi(this);

    connect(ui->valueSlider, &QSlider::valueChanged, [=](int value){
        ui->valueSlider->setValue(value);
        ui->valueDoubleSpinBox->setValue((static_cast<double>(value)/m_command->divider()));
    });
    connect(ui->valueDoubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          [=](double value){
        ui->valueSlider->setValue(static_cast<quint16>(qRound(value*m_command->divider())));
        ui->valueDoubleSpinBox->setValue(value);
    });
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveResult()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(hide()));
    setStruct();
}

CalibrateDialog::~CalibrateDialog()
{
    delete ui;
}

void CalibrateDialog::setStruct() {
    auto divider = m_command.data()->divider();

    ui->titleLabel->setText(m_calibraion.data()->name);
    ui->valueSlider->setMinimum(qRound(m_calibraion.data()->min*divider));
    ui->valueSlider->setMaximum(qRound(m_calibraion.data()->max*divider));
    ui->valueDoubleSpinBox->setSingleStep(1.0/divider);
    int precision = qRound(0.43429*qLn(divider));
    ui->valueDoubleSpinBox->setDecimals(precision);

    ui->valueDoubleSpinBox->setMinimum(m_calibraion.data()->min);
    ui->valueDoubleSpinBox->setMaximum(m_calibraion.data()->max);

    ui->minLabel->setText(QString::number(m_calibraion.data()->min, 'f', precision));
    ui->maxLabel->setText(QString::number(m_calibraion.data()->max, 'f', precision));

    ui->valueSlider->setValue(qRound(m_command.data()->valueDouble()*divider));
    ui->valueDoubleSpinBox->setValue(m_command.data()->valueDouble());
}

void CalibrateDialog::saveResult() {
    int value = ui->valueSlider->value();
    if(!m_command.isNull()) {
        m_command.data()->setFromWidget(value/m_command.data()->divider());
    }
    this->hide();
}

QString CalibrateDialog::getName(){
    return m_calibraion.data()->name;
}


