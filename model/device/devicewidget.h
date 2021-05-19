#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>
#include <QMap>
#include <QVariant>
//#include "../../device/device.h"


class DeviceWidget : public QWidget
{
    Q_OBJECT
public:
    DeviceWidget(/*Device* device*/);

public slots:
    virtual void setValue(quint16 reg, double value);
    virtual void setValue(quint16 reg, int value);

signals:
    void dataChanged(quint16 reg, double value);
    void dataChanged(quint16 reg, int value);

private:
//    Device* m_device;
    QMap<quint16, QVariant> m_values;
//    Q_Map<quint16, >

};

#endif // DEVICEWIDGET_H
