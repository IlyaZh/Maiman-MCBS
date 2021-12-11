#ifndef CalibrateDialog_H
#define CalibrateDialog_H

#include <QDialog>
#include "device/commandsettings.h"

class CalibrationAndLimitsWidget;
struct CalibrationKoef;

namespace Ui {
class CalibrateDialog;
}

class CalibrateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrateDialog(QVector<CalibrationAndLimitsWidget*> calibrations,QVector<CalibrationAndLimitsWidget*> limits, QWidget *parent = nullptr);
    ~CalibrateDialog();
    void setValues();
private:
    QVector<CalibrationAndLimitsWidget*> m_calibrationWidgets;
    QVector<CalibrationAndLimitsWidget*> m_limitsWidgets;
    Ui::CalibrateDialog *ui;
private slots:
    void saveResult();

signals:
    void sendData(QString);
};

#endif // CalibrateDialog_H