#include "mainfacade.h"

MainFacade::MainFacade(MainWindow& window, GuiFactory& factory, QObject* parent) :
    QObject(parent),
    window(window),
    factory(factory)
{
    factory.start();
}

MainFacade::~MainFacade() {

}

//void MainFacade::eventHandle(const QString& event, const QVariant& value) {

//}

void MainFacade::createWidgetFor(Device& device) {}

void MainFacade::clear() {
    // TODO: сделай функцию очистки созданных виджетов
}
