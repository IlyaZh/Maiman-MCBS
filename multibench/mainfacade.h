#ifndef MAINFACEDE_H
#define MAINFACEDE_H

#include <QObject>
#include <QWidget>
#include <QVector>

class GuiFactory;
class MainWindow;
class Device;
class DeviceWidget;

class MainFacade : public QObject
{
    Q_OBJECT
public:
    explicit MainFacade(MainWindow& window, GuiFactory& factory, QObject* parent = nullptr);
    ~MainFacade();
    void createWidgetFor(Device* device);
    void setBaudRates(const QStringList& bauds);
private:
    MainWindow& m_window;
    GuiFactory& m_factory;

};

#endif // MAINFACEDE_H
