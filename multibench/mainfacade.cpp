#include "mainfacade.h"
#include <QDebug>

#include "model/device/devicewidget.h"
#include "device/device.h"
#include "mainwindow.h"
#include "model/guifactory.h"
#include "widgets/calibratedialog.h"

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
    QVector<CalibrateDialog*> calibVec;
    if(widget) {
        widget->setAddress(static_cast<int>(device->addr()));
        connect(device, &Device::linkChanged, widget, &DeviceWidget::setLink);
        m_window.addDeviceWidget(widget);
        auto& calibration = m_factory.getDeviceWidgetDesc(device->id())->calibration;
        for (auto& item:calibration){
            for (auto command:device->commands()){
                if(command->code() == item.code)
                    calibVec.append(new CalibrateDialog(&item, command));
            }
        }
        m_window.addCalibrateWidget(device->addr(),calibVec);
    } else {
        qWarning() << "Can't find device widget with id=" << device->id();
    }
}

void MainFacade::setBaudRates(const QStringList& bauds) {
    m_window.setBaudRates(bauds);
}
