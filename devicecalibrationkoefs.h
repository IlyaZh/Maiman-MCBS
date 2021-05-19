#ifndef DEVICECALIBRATIONKOEFS_H
#define DEVICECALIBRATIONKOEFS_H

#include <QObject>
#include <QPointer>
#include <QString>
#include "device/devicecommand.h"

class DeviceCalibrationKoef : public QObject {
    Q_OBJECT
public:
    explicit DeviceCalibrationKoef(DeviceCommand* command, QString title, int min, int max, QObject* parent = nullptr)
        : QObject(parent), m_cmd(command), m_title(title), m_min(min), m_max(max)
    {

    }
    ~DeviceCalibrationKoef() {
        if(m_cmd) m_cmd->disconnect();
    }
    const DeviceCommand* command() {}
    QString title() {}
    int min() {}
    int max() {}

public slots:
    void setNewValue(int) {}
private:
    QPointer<DeviceCommand> m_cmd;
    QString m_title;
    int m_min;
    int m_max;
};

#endif // DEVICECALIBRATIONKOEFS_H
