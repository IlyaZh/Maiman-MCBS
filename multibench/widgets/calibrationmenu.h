#ifndef CalibrateDialog_H
#define CalibrateDialog_H

#include <QDialog>
#include "device/commandsettings.h"
#include "model/device/devicewidget.h"

class CalibrationAndLimitsWidget;
struct CalibrationKoef;

namespace Ui {
class CalibrationMenu;
}

class CalibrationMenu : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationMenu(quint16 id,QMap<quint16, DeviceWidgetDesc> deviceDesc, const QMap<quint16, QSharedPointer<DevCommand>>& commands, QWidget *parent = nullptr);
    ~CalibrationMenu();
    void setValues();
private:

    QVector<CalibrationAndLimitsWidget*> m_calibrationWidgets;
    QVector<CalibrationAndLimitsWidget*> m_limitsWidgets;
    Ui::CalibrationMenu *ui;
    quint16 m_id;
    const QMap<quint16, QSharedPointer<DevCommand>>& m_commands;
    QMap<quint16, DeviceWidgetDesc> m_deviceWidgets;

    static const QString styleButtonOn;
    static const QString styleButtonOff;
private slots:
    void saveResult();
    void widgetsAreValid();

signals:
    void sendData(QString);
};

#endif // CalibrateDialog_H
