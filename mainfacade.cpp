#include "mainfacade.h"
#include <QDebug>

MainFacade::MainFacade(MainWindow& window, GuiFactory& factory, QObject* parent) :
    QObject(parent),
    m_window(window),
    m_factory(factory)
{
    factory.start();
}

MainFacade::~MainFacade() {

}

//void MainFacade::eventHandle(const QString& event, const QVariant& value) {

//}

void MainFacade::createWidgetFor(Device& device) {
    auto widget = m_factory.createWidget(device.id());
    if(widget != nullptr) {
        m_widgets.append(QSharedPointer<DeviceWidget>(widget));
        m_window.addDeviceWidget(widget);
    } else {
        qWarning() << "Can't find device widget with id=" << device.id();
    }
}

void MainFacade::clear() {
    for(auto widget = m_widgets.begin(); widget != m_widgets.end(); ++widget) {
        widget->get()->disconnect();
        widget->reset();
    }
    m_widgets.clear();
}
