#include "mainwindow.h"
#include "mainwindowcontroller.h"

#include <QApplication>
#include "modbus/modbus.h"
#include "network/networkmodel.h"

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

    MainWindowController mwCntrl;
    w.addController(mwCntrl);

    NetworkModel* model = new NetworkModel();
    mwCntrl.addModel(model);
    QVariant var;
    var.setValue((double) 0xfffe/10);

    qDebug() << var.toUInt() << var.toDouble();

    return a.exec();
}
