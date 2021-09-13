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
//    void eventHandle(const QString& event, const QVariant& value);
    void createWidgetFor(Device& device);
    void clear();
private:
    MainWindow& m_window;
    GuiFactory& m_factory;
    QVector<DeviceWidget*> m_widgets;

};

#endif // MAINFACEDE_H
