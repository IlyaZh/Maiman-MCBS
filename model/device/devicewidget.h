#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>
#include <QMap>
#include <QVariant>
#include <QPointer>
#include "device/device.h"

class DeviceWidget : public QWidget
{
    Q_OBJECT
public:
    DeviceWidget(Device *pDev);
    QString devName();

public slots:
    virtual void setValue(quint16 reg, double value);
    virtual void setValue(quint16 reg, int value);

signals:
    void dataChanged(quint16 reg, double value);
    void dataChanged(quint16 reg, int value);

private:
    QMap<quint16, QVariant> m_values;
    Device* m_device;

};

#endif // DEVICEWIDGET_H
