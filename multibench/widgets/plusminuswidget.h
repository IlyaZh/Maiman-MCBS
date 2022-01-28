#ifndef PLUSMINUSWIDGET_H
#define PLUSMINUSWIDGET_H

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

class PlusMinusWidget : public QDialog
{
    Q_OBJECT

public:
    explicit PlusMinusWidget(const CalibrationKoef& calibration, QSharedPointer<DevCommand> command,QWidget *parent = nullptr);
    explicit PlusMinusWidget(const Limit& limit,QSharedPointer<DevCommand> command,QSharedPointer<DevCommand> maxCommand,QSharedPointer<DevCommand> minCommand,QWidget *parent = nullptr);
    ~PlusMinusWidget();
    void sendValue();
    bool state();
    double value();
    void setMin(double min);
    void setMax(double max);
private slots:
    void increment();
    void decrement();

    void editedValue();
    void rejectedEdit();
    void inputCompleted();

signals:
    void editFinished();

private:
    void keyPressEvent(QKeyEvent *event) override;

    QSharedPointer<DevCommand> m_command;
    Ui::CalibrationAndLimitsWidget *ui;
    QDoubleValidator* m_validator;

    double minValue = 0;
    double maxValue = 0;
    double delta = 0;
    bool m_state = true;

    static const QString styleSheetOK;
    static const QString styleSheetERROR;
};

#endif // PLUSMINUSWIDGET_H
