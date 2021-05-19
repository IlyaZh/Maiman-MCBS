#ifndef PARAMETERWIDGET_H
#define PARAMETERWIDGET_H

#include <QWidget>
#include "device/devicecommand.h"

namespace Ui {
class ParameterWidget;
}

class ParameterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterWidget(QString title, DeviceCommand* value, DeviceCommand* min, DeviceCommand* max, DeviceCommand* real, QWidget* parent);
    ~ParameterWidget();

private slots:
    void minChanged();
    void maxChanged();
    void valueChanged();
    void realChanged();

private:
    Ui::ParameterWidget *ui;
    QString title;
    DeviceCommand* min;
    DeviceCommand* max;
    DeviceCommand* value;
    DeviceCommand* real;
};

#endif // PARAMETERWIDGET_H
