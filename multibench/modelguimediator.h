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
    explicit ModelGuiMediator(MainWindow& window, GuiFactory& factory,NetworkModel& networkModel, QObject* parent = nullptr);

    //void setBaudRates();
private:
    MainWindow& m_window;
    GuiFactory& m_factory;
    NetworkModel& m_network;
    QPointer<DataSource> m_device;

public slots:
    void createWidgetFor(Device* device);
    void createCalibAndLimitsWidgets(quint8 addr, quint16 id);
private slots:
    void refreshComPorts();
    void connectToNetwork(QVariant value);
    void rescan();
    void setBaudrateToWindow(QStringList baud);
signals:
    void rescanNetwork();

};

#endif // MODELGUIMEDIATOR_H
