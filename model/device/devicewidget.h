#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>
#include <QMap>

class ParameterWidget;

struct Content {
    QString fileName;
    QString description;
    QString link;
};

struct Control {
    quint16 min = 0;
    quint16 max = 0;
    quint16 value = 0;
    quint16 real = 0;
    QString name;
};

struct Limit {
    quint16 code;
    double minValue = 0;
    double maxValue = 0;
    quint16 minCode = 0;
    quint16 maxCode = 0;
    QString name;
};

struct CalibrationKoef {
    quint16 code;
    double min;
    double max;
    QString name;
};

struct Checkbox {
    quint16 code = 0;
    quint16 onCommand = 0;
    quint16 offCommand = 0;
    quint16 mask = 0;
    QString name;
};

struct Button {
    QString name;
    quint16 code = 0;
    quint16 mask = 0;
    quint16 onCommand = 0;
    quint16 offCommand = 0;
};

struct LedMask {
    quint16 code = 0;
    quint16 mask = 0;
    QColor defaultColor;
    QColor maskColor;
    QString msg;
};

struct Led {
    QString name;
    QVector<LedMask> ledMasks;
};

struct DeviceWidgetDesc {
    uint id;
    QString name = "Unknown device";
    Content content;
    QVector<Limit> limits;
    QVector<CalibrationKoef> calibration;
    QVector<Control> controls;
    QVector<Checkbox> checkboxes;
    QVector<Button> buttons;
    QVector<Led> leds;
};

namespace Ui {
class DeviceWidget;
}

class DeviceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceWidget(const DeviceWidgetDesc& description, QWidget *parent = nullptr);
    ~DeviceWidget();
    void setAddress(int addr);

public slots:
    void setValue(quint16 reg, double value);
    void parameterChanged(quint16 reg, double value);
//    void setValue(quint16 reg, int value);

signals:
    void dataChanged(quint16 reg, double value);
//    void dataChanged(quint16 reg, int value);

private:
    Ui::DeviceWidget *ui;
    const DeviceWidgetDesc& description;
    QMap<quint16, QVariant> m_values;
    QVector<ParameterWidget*> m_paramWidgets;
};

#endif // DEVICEWIDGET_H
