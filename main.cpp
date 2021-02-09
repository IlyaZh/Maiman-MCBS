#include "mainwindow.h"
#include "mainwindowcontroller.h"

#include <QApplication>
#include "modbus/modbus.h"
#include "network/networkmodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    MainWindowController mwCntrl;
    w.addController(mwCntrl);

    NetworkModel* model = new NetworkModel();
    mwCntrl.addModel(model);

    return a.exec();
}
