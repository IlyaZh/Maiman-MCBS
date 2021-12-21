#ifndef CalibrateDialog_H
#define CalibrateDialog_H

#include <QDialog>
#include "device/commandsettings.h"
#include "model/device/devicewidget.h"

class CalibrationAndLimitsWidget;
struct CalibrationKoef;

namespace Ui {
class CalibrationDialog;
}

class CalibrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationDialog(const DeviceWidgetDesc& deviceDesc, const QMap<quint16, QSharedPointer<DevCommand>>& commands, QWidget *parent = nullptr);
    ~CalibrationDialog();
    void setValues();
private:

    QVector<CalibrationAndLimitsWidget*> m_calibrationWidgets;
    QVector<CalibrationAndLimitsWidget*> m_limitsWidgets;
    Ui::CalibrationDialog *ui;
    const QMap<quint16, QSharedPointer<DevCommand>>& m_commands;
    const DeviceWidgetDesc& m_deviceWidgetsDesc;

    static const QString styleButtonOn;
    static const QString styleButtonOff;
private slots:
    void saveResult();
    void widgetsAreValid();

signals:
    void sendData(QString);
};

#endif // CalibrateDialog_H
