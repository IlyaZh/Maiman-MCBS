#include "model/device/devicewidget.h"

DeviceWidget::DeviceWidget(Device* pDev) :
    m_device(pDev)
{
    connect(m_device, SIGNAL(dataToView(quint16, double)), this, SLOT(dataChanged(quint16, double)));
    connect(m_device, SIGNAL(dataToView(quint16, int)), this, SLOT(dataChanged(quint16, int)));
}

QString DeviceWidget::devName() {
    return m_device->name();
}

void DeviceWidget::setValue(quint16 reg, double value) {}

void DeviceWidget::setValue(quint16 reg, int value) {}
