#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QtWidgets>

class DevCommand;
class ControlWidget;
class BinaryWidget;
class HiddenWidget;

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
    QMap<quint16, Button> buttons;
    QVector<Led> leds;
};

namespace Ui {
class DeviceWidget;
}

class DeviceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceWidget(const DeviceWidgetDesc& description, const QMap<quint16, QSharedPointer<DevCommand>>& commands,  QWidget *parent = nullptr);
    ~DeviceWidget();
    void setAddress(int addr);

public slots:
    void setLink(bool link);

signals:
    void sizeChanged(const QSize& size);

private:
    Ui::DeviceWidget *ui;
    const DeviceWidgetDesc& m_description;
    QMap<quint16, QVariant> m_values;
    QVector<HiddenWidget*> m_widgets;
    const QMap<quint16, QSharedPointer<DevCommand>>& m_commands;
    QGridLayout* m_widgetLayout;
    QMap<quint16, BinaryWidget*> m_binaryWidgets;
    QVector<QPushButton*> m_pinButtons;
    bool m_hideControls {false};

    void paintEvent(QPaintEvent*) override;
    void adjust();
    void resizeEvent(QResizeEvent *event) override;

    static const QString linkStyleOn;
    static const QString linkStyleOff;

private slots:
    void setLaserButton(quint16 value);
    void setTecButton(quint16 value);
    void laserButtonClicked(bool state);
    void tecButtonClicked(bool state);
    void hideControlsButtonClicked(bool flag);
    void pinButtonClicked(int idx, bool state);
};

#endif // DEVICEWIDGET_H
