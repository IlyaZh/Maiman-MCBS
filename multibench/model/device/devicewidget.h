#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QtWidgets>
#include "widgets/inlineedit.h"

class DevCommand;
class ControlWidget;
class BinaryWidget;
class HiddenWidget;
class DeviceCondition;

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
    bool fixed {false};
};

struct Limit {
    quint16 code;
    double minValue {0};
    double maxValue {0};
    quint16 minCode {0};
    quint16 maxCode {0};
    quint16 pairCode {0};
    bool isPairMin {true};
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
    void setConstraint(bool state);
public slots:
    void setLink(bool link);
signals:
    void nameEdited(QString name, int addr);

private:
    Ui::DeviceWidget *ui;
    const QMap<quint16, Button> &m_buttons;
    QMap<quint16, QVariant> m_values;
    QVector<HiddenWidget*> m_widgets;
    QMap<quint16, QSharedPointer<DevCommand>> m_commands;
    QGridLayout* m_widgetLayout;
    QMap<quint16, BinaryWidget*> m_binaryWidgets;
    QVector<QPushButton*> m_pinButtons;
    bool m_hideControls {false};
    QPushButton* m_laserButton {nullptr};
    QPushButton* m_tecButton {nullptr};
    DeviceCondition* m_deviceCondition;
    int m_fixedWidgets {0};

    void paintEvent(QPaintEvent*) override;
    void adjust();

    static const QString linkStyleOn;
    static const QString linkStyleOff;
    static const QString labelEnableStyle;
    static const QString labelDisableStyle;

private slots:
    void setLaserButton(quint16 value);
    void setTecButton(quint16 value);
    void laserButtonClicked();
    void tecButtonClicked();
    void hideControlsButtonClicked(bool flag);
    void pinButtonClicked(int idx, bool state);
};

#endif // DEVICEWIDGET_H
