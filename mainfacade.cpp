#include "mainfacade.h"
#include <QDebug>

#include "model/device/devicewidget.h"
#include "device/device.h"
#include "mainwindow.h"
#include "model/guifactory.h"

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
    auto widget = m_factory.createWidget(device.id(), device.commands());
    if(widget != nullptr) {
        m_widgets.append(widget);

        m_window.addDeviceWidget(widget);
    } else {
        qWarning() << "Can't find device widget with id=" << device.id();
    }
}

void MainFacade::clear() {
    for(auto widget : m_widgets) {
        widget->disconnect();
        widget->deleteLater();
    }
    m_widgets.clear();
}
