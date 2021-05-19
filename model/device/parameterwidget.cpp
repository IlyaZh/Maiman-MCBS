#include "parameterwidget.h"
#include "ui_parameterwidget.h"

ParameterWidget::ParameterWidget(QString title, DeviceCommand* value, DeviceCommand* min, DeviceCommand* max, DeviceCommand* real, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ParameterWidget)
{
    ui->setupUi(this);

    this->title = title;
    this->value = value;
    this->min = min;
    this->max = max;
    this->real = real;

    if(this->value != nullptr) {
        connect(this->value, &DeviceCommand::valueChanged, this, &ParameterWidget::valueChanged);
    }
}

ParameterWidget::~ParameterWidget()
{
    delete ui;
}

void ParameterWidget::minChanged() {

}

void ParameterWidget::maxChanged() {}

void ParameterWidget::valueChanged() {}

void ParameterWidget::realChanged() {}
