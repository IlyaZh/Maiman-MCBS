#ifndef GUIFACTORY_H
#define GUIFACTORY_H

#include <QObject>
#include <QtWidgets>
#include "factories/parserworker.h"
#include "device/devicewidget.h"
#include "widgets/calibrationandlimitswidget.h"
#include "widgets/calibratedialog.h"

class DevCommand;

class GuiFactory : public QObject
{
    Q_OBJECT
public:
    explicit GuiFactory(const QString& fileName, QObject *parent = nullptr);
    void start();
    DeviceWidget* createDeviceWidget(quint16 id, const QMap<quint16, QSharedPointer<DevCommand>>& commands);
    CalibrateDialog* createCalibrationAndLimitsDialog(quint16 id, const QMap<quint16, QSharedPointer<DevCommand>>& commands);

private slots:
    void parsingFinished();
    void threadError(const QString&);

    QVector<CalibrationAndLimitsWidget*> createDeviceCalibrationWidget(quint16 id, const QMap<quint16, QSharedPointer<DevCommand>>& commands);
    QVector<CalibrationAndLimitsWidget*> createDeviceLimitsWidget(quint16 id, const QMap<quint16, QSharedPointer<DevCommand>>& commands);

signals:

private:
    QPointer<QThread> m_thread;
    QPointer<ParserWorker> m_parseWorker;
    QString m_fileName;

    QMap<quint16, DeviceWidgetDesc> m_deviceWidgets;

    bool parseTree(const TreeItem& tree);
    DeviceWidgetDesc parseDevice(const TreeItem& item);
    Content parseDeviceContent(const TreeItem& item);
    Limit parseDeviceLimit(const TreeItem& item);
    CalibrationKoef parseCalibrationKoef(const TreeItem& item);
    Control parseParamControls(const TreeItem& item);
    Checkbox parseCheckboxes(const TreeItem& item);
    Button parseButtons(const TreeItem& item);
    Led parseLeds(const TreeItem& item);
};

#endif // GUIFACTORY_H
