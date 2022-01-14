#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMap>
#include <QValidator>
#include "device/commandsettings.h"

struct Control;

namespace Ui {
class CommandWidget;
}

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QStringView name,
                           QSharedPointer<DevCommand> Value,
                           QSharedPointer<DevCommand> Max,
                           QSharedPointer<DevCommand> Min,
                           QSharedPointer<DevCommand> Real,
                           QWidget *parent = nullptr);
    ~ControlWidget();

signals:
//    void valueChanged(quint16 reg, double value);

private slots:
    void setValue(/*double value, int decimal*/);
    void setMaxValue(/*double value, int decimal*/);
    void setMinValue(/*double value, int decimal*/);
    void setRealValue(/*double value, int decimal*/);

private:
    Ui::CommandWidget *ui;
//    const Control& m_settings;
    QDoubleValidator* m_Validator;
    QVector<DevCommand> m_Commands;
    QSharedPointer<DevCommand> m_Real;
    QSharedPointer<DevCommand> m_Value;
    QSharedPointer<DevCommand> m_Max;
    QSharedPointer<DevCommand> m_Min;
    bool isUserEdit = false;

    void setUnits(QString units);

    void adjust();
private slots:
    void userEnteredValue();
};

#endif // CONTROLWIDGET_H
