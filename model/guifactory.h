#ifndef GUIFACTORY_H
#define GUIFACTORY_H

#include <QThread>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QVector>
#include <QColor>
#include <QMap>
#include "factories/parserworker.h"
#include "appsettings.h"
#include "device/devicewidget.h"

class DevCommand;

class GuiFactory : public QObject
{
    Q_OBJECT
public:
    GuiFactory(QString fileName, AppSettings& settings, QObject *parent = nullptr);
    ~GuiFactory();
    void start();
    DeviceWidget* createWidget(quint16 id, const QMap<quint16, QSharedPointer<DevCommand>>& commands);



private slots:
    void parsingFinished();
    void threadError(const QString&);

signals:

private:
    QThread* m_thread;
    QPointer<ParserWorker> m_parseWorker;
    AppSettings& m_settings;
    QString m_fileName;

    QMap<quint16, QSharedPointer<DeviceWidgetDesc>> m_deviceWidgets;

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
