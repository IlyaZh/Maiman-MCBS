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

void MainFacade::createWidgetFor(Device* device) {
    QPointer<DeviceWidget> widget(m_factory.createWidget(device->id(), device->commands()));
    if(widget) {
        m_window.addDeviceWidget(widget);
    } else {
        qWarning() << "Can't find device widget with id=" << device->id();
    }
}

void MainFacade::setBaudRates(const QStringList& bauds) {
    m_window.setBaudRates(bauds);
}
