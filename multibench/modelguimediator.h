#ifndef MODELGUIMEDIATOR_H
#define MODELGUIMEDIATOR_H

#include <QObject>
#include <QWidget>
#include <QVector>
#include "mainwindow.h"
#include "network/networkmodel.h"

class GuiFactory;
class MainWindow;
class Device;
class DeviceWidget;
class CalibrationAndLimitsWidget;

class ModelGuiMediator : public QObject
{
    Q_OBJECT
public:
    explicit ModelGuiMediator(MainWindow& window, GuiFactory& factory, NetworkModel& networkModel, QObject* parent = nullptr);

private:
    MainWindow& m_window;
    GuiFactory& m_factory;
    NetworkModel& m_network;
    QMap<quint8, quint16> m_listOfCalibrationDialog;

private slots:
    void createWidgetFor(Device* device);
    void createCalibAndLimitsWidgets(quint8 addr, quint16 id);
private slots:
    void refreshComPorts();
    void changeConnectState(PortType type, QVariantMap value);
    void rescan();
    void setBaudrateToWindow(QStringList baud);
signals:
    void rescanNetwork();
    void deletedCalibrationDialog();

};

#endif // MODELGUIMEDIATOR_H
