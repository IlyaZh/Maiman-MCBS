#ifndef CalibrateDialog_H
#define CalibrateDialog_H

#include <QDialog>
#include "device/commandsettings.h"

struct CalibrationKoef;

namespace Ui {
class CalibrateDialog;
}

class CalibrateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrateDialog(CalibrationKoef* calibration, QSharedPointer<DevCommand> command, QWidget *parent = nullptr);
    ~CalibrateDialog();
    void setStruct();
    QString getName();
private:
    QSharedPointer<CalibrationKoef> m_calibraion;
    QSharedPointer<DevCommand> m_command;
    Ui::CalibrateDialog *ui;
    int divider;

private slots:
    void saveResult();

signals:
    void sendData(QString);
};

#endif // CalibrateDialog_H
