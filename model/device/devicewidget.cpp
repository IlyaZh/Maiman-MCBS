#include "devicewidget.h"
#include "ui_devicewidget.h"

DeviceWidget::DeviceWidget(const DeviceWidgetDesc& description, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceWidget),
    description(description)
{
    ui->setupUi(this);

    //    connect(m_device, SIGNAL(dataToView(quint16, double)), this, SLOT(dataChanged(quint16, double)));
    //    connect(m_device, SIGNAL(dataToView(quint16, int)), this, SLOT(dataChanged(quint16, int)));
}

DeviceWidget::~DeviceWidget()
{
    delete ui;
}

QString DeviceWidget::devName() {
//    return m_device->name();
}

void DeviceWidget::setValue(quint16 reg, double value) {}

void DeviceWidget::setValue(quint16 reg, int value) {}
