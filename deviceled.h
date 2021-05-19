#ifndef DEVICELED_H
#define DEVICELED_H

#include <QObject>
#include <QWidget>
#include <QVector>
#include <QPointer>
#include <QColor>
#include "device/devicecommand.h"

class LedMask;

class DeviceLed : public QWidget
{
    Q_OBJECT
public:
    static const QString LED_DEFAULT_COLOR;
    static const QString LED_DEFAULT_ACTIVE_COLOR;
    explicit DeviceLed(QString title, LedMask* maskObj, QString defaultColor, QWidget *parent = nullptr);
    explicit DeviceLed(QString title, QVector<LedMask*> masks, QString defaultColor, QWidget *parent = nullptr);
    ~DeviceLed();
    QString title() { return m_title; }
    QWidget* widget() { return m_widget; }

    void checkMasks();
signals:

public slots:

private:
    void commonConstructor();
    QString m_title;
    QVector<LedMask*> m_masks;
    QColor m_defaultColor;
    QPointer<QWidget> m_widget;
};

class LedMask : public QObject {
    Q_OBJECT
public:
    explicit LedMask(DeviceLed* ledParent, DeviceCommand* command, QString message, quint16 mask, QString activeColor = DeviceLed::LED_DEFAULT_ACTIVE_COLOR, QObject* parent = nullptr);
    ~LedMask();
    bool maskOccured() { return m_maskOccured; }
    const DeviceCommand* command() { return m_cmd; }
    const QString message() { return m_msg; }
    quint16 mask() { return m_mask; }
    QColor activeColor() { return m_activeColor; }
private:
    QPointer<DeviceLed> m_ledParent;
    QPointer<DeviceCommand> m_cmd;
    QString m_msg;
    quint16 m_mask;
    QColor m_activeColor;
    bool m_maskOccured;
};

#endif // DEVICELED_H
