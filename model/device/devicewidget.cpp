#include "devicewidget.h"
#include "ui_devicewidget.h"
#include <QDebug>

#include "model/device/parameterwidget.h"

DeviceWidget::DeviceWidget(const DeviceWidgetDesc& description, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceWidget),
    description(description)
{
    ui->setupUi(this);

    for(auto control : description.controls) {
        auto widget = new ParameterWidget(control, this);
        m_paramWidgets.append(widget);
        ui->paramLayout->addWidget(widget);
        connect(widget, &ParameterWidget::valueChanged, this, &DeviceWidget::parameterChanged);
    }

    //    connect(m_device, SIGNAL(dataToView(quint16, double)), this, SLOT(dataChanged(quint16, double)));
    //    connect(m_device, SIGNAL(dataToView(quint16, int)), this, SLOT(dataChanged(quint16, int)));
}

DeviceWidget::~DeviceWidget()
{
    delete ui;
}

void DeviceWidget::setAddress(int addr) {
    ui->addrLabel->setNum(addr);
}

// public slots


void DeviceWidget::setValue(quint16 reg, double value) {
    qDebug() << "DeviceWidget" << reg << value;
    for (auto paramWid : m_paramWidgets) {
        paramWid->setValue(reg, value);
    }
}

void DeviceWidget::parameterChanged(quint16 reg, double value) {
    qDebug() << "parameterChanged" << reg << value;
    emit dataChanged(reg, value);
}

//void DeviceWidget::setValue(quint16 reg, int value) {}
