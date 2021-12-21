#ifndef CALIBRATIONANDLIMITSWIDGET_H
#define CALIBRATIONANDLIMITSWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QLayout>
#include "device/commandsettings.h"
#include <QDoubleValidator>

struct CalibrationKoef;
struct Limit;
namespace Ui {
class CalibrationAndLimitsWidget;
}

class CalibrationAndLimitsWidget : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationAndLimitsWidget(const CalibrationKoef& calibration, QSharedPointer<DevCommand> command,QWidget *parent = nullptr);
    explicit CalibrationAndLimitsWidget(const Limit& limit,QSharedPointer<DevCommand> command,QSharedPointer<DevCommand> maxCommand,QSharedPointer<DevCommand> minCommand,QWidget *parent = nullptr);
    ~CalibrationAndLimitsWidget();
    void sendValue();
    bool getState();
private slots:
    void increment();
    void decrement();

    void editedValue();
    void rejectedEdit();

signals:
    void editFinished();

private:
    QSharedPointer<DevCommand> m_command;
    QSharedPointer<DevCommand> m_limitMaxCommand;
    QSharedPointer<DevCommand> m_limitMinCommand;
    Ui::CalibrationAndLimitsWidget *ui;
    QDoubleValidator* m_validator;
    //const CalibrationKoef& m_calibration;
    //const Limit& m_limit;

    double minValue=0;
    double maxValue=0;
    bool m_state = true;

    static const QString styleSheetOK;
    static const QString styleSheetERROR;
};

#endif // CALIBRATIONANDLIMITSWIDGET_H
