#ifndef COMMANDWIDGET_H
#define COMMANDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMap>
#include <QValidator>
#include "device/commandsettings.h"

struct Control;

namespace Ui {
class CommandWidget;
}

class CommandWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommandWidget(QStringView name,
                           QSharedPointer<DevCommand> Value,
                           QSharedPointer<DevCommand> Max,
                           QSharedPointer<DevCommand> Min,
                           QSharedPointer<DevCommand> Real,
                           QWidget *parent = nullptr);
    ~CommandWidget();

signals:
//    void valueChanged(quint16 reg, double value);

private:
    Ui::CommandWidget *ui;
//    const Control& m_settings;
    QDoubleValidator m_Validator;
    QVector<DevCommand> m_Commands;
    QSharedPointer<DevCommand> m_Real;
    QSharedPointer<DevCommand> m_Value;
    QSharedPointer<DevCommand> m_Max;
    QSharedPointer<DevCommand> m_Min;

    void setValue(double value, int decimal);
    void setMaxValue(double value, int decimal);
    void setMinValue(double value, int decimal);
    void setRealValue(double value, int decimal);
    void setUnits(QString units);

    void adjust();
private slots:
    void LineEdit_EditFinished();
};

#endif // COMMANDWIDGET_H
