#include "mainwindow.h"
#include "interfaces/mainwindowcontrollerinterface.h"

#include <QApplication>
#include "network/networkmodel.h"
#include "network/protocols/modbus.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ////========================
    /// 1. создаем экземпляры парсеров
    ///     а. парсер данных
    ///     б. парсер gui
    /// 2. парсим данные
    /// 3. Создаем экземпляры MainWindow, Controller и Model
    /// 4. передаем данные gui в MainWindow и в DeviceModel
    ////========================

    MainWindow w;
    w.show();

//    MainWindowController mwCntrl;
//    w.addController(mwCntrl);

    Modbus* modbus = new Modbus("modbus");
    NetworkModel* model = new NetworkModel(modbus);
//    mwCntrl.addModel(model);

    return a.exec();
}
