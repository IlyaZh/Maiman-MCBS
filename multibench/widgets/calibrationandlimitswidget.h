#ifndef CALIBRATIONANDLIMITSWIDGET_H
#define CALIBRATIONANDLIMITSWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QLayout>
#include "device/commandsettings.h"
struct CalibrationKoef;
struct Limit;
namespace Ui {
class CalibrationAndLimitsWidget;
}

class CalibrationAndLimitsWidget : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationAndLimitsWidget(CalibrationKoef* calibration, QSharedPointer<DevCommand> command,QWidget *parent = nullptr);
    explicit CalibrationAndLimitsWidget(Limit* limit,QSharedPointer<DevCommand> command,QSharedPointer<DevCommand> limitCommand,QWidget *parent = nullptr);
    ~CalibrationAndLimitsWidget();
    void sendValue();
private slots:
    void increment();
    void decrement();

private:
    CalibrationKoef* m_calibration;
    Limit* m_limit;
    QSharedPointer<DevCommand> m_command;
    QSharedPointer<DevCommand> m_limitCommand;
    Ui::CalibrationAndLimitsWidget *ui;

    static const QString nameStyle;
    static const QString minorParametersStyle;
    static const QString buttonsStyle;
    static const QString lineEditStyle;
};

#endif // CALIBRATIONANDLIMITSWIDGET_H
