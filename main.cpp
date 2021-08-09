#include "mainwindow.h"
#include "interfaces/mainwindowcontrollerinterface.h"

#include <QApplication>
#include "network/networkmodel.h"
#include "network/protocols/modbusprotocol.h"
#include "model/devicefactory.h"
#include "model/guifactory.h"
#include <QTcpSocket>
#include "globals.h"
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "factories/Parser.h"
#include "datasource.h"
#include "windownetworkmediator.h"

#include <QDebug>

QLocale wlocale;
bool debugMode = false;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationVersion(QString::number(Constants::MajorVersion) + "." + QString::number(Constants::MinorVersion) + "." + QString::number(Constants::PatchVersion));
    QApplication::setFont(Constants::ApplicationDefaultFont);

  AppSettings settings;

#ifdef QT_DEBUG
    debugMode = true;
#endif

    QCommandLineParser cliParser;
    QCommandLineOption debugOption(QStringList() << "d" << "debug");
    cliParser.addOption(debugOption);
    cliParser.parse(QCoreApplication::arguments());
    if(cliParser.isSet(debugOption)) {
        debugMode = true;
    }

    wlocale = QLocale(QLocale::system());

    ////========================
    /// 1. создаем экземпляры парсеров
    ///     а. парсер данных
    ///     б. парсер gui
    /// 2. парсим данные
    /// 3. Создаем экземпляры MainWindow, Controller и Model
    /// 4. передаем данные gui в MainWindow и в DeviceModel
    ////========================

    MainWindow w(settings);
    w.setFont(Constants::ApplicationDefaultFont);
    w.show();

    DataSource dataSource;

    GuiFactory guiFactory("DeviceGUI.xml", settings);
//    MainViewFacade mvCntrl(dataSource, settings, guiFactory);
//    mvCntrl.addView(&w);

    DeviceFactory deviceFactory("DeviceDB.xml", settings);
    ModbusProtocol modbus(&dataSource, SoftProtocol::TimeoutDefault, 500);

    NetworkModel model(deviceFactory, modbus);
//    model.addFacade(mvCntrl);

    WindowNetworkMediator windowNetworkMediator(w, model, settings);

    return app.exec();
}
