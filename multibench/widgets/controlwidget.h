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

private slots:
    void setValue();
    void setMaxValue();
    void setMinValue();
    void setRealValue();
    void setUnits(QStringView units);

private:
    Ui::CommandWidget *ui;
    QDoubleValidator* m_Validator;
    QVector<DevCommand> m_Commands;
    QSharedPointer<DevCommand> m_Real;
    QSharedPointer<DevCommand> m_Value;
    QSharedPointer<DevCommand> m_Max;
    QSharedPointer<DevCommand> m_Min;
    bool isUserEdit = false;



    void adjust();
private slots:
    void userEnteredValue();
};

#endif // CONTROLWIDGET_H
