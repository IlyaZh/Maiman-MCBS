#include "mainwindow.h"
//#include "interfaces/mainwindowcontrollerinterface.h"

#include <QApplication>
#include <QLocale>
#include "network/networkmodel.h"
#include "network/protocols/modbusprotocol.h"
#include "model/devicefactory.h"
#include "model/guifactory.h"
#include <QTcpSocket>
#include "constants.h"
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "factories/Parser.h"
#include "datasource.h"
#include "maincontroller.h"
#include "mainfacade.h"
//#include "DebugMode.h"
#include <QFontDatabase>

#include <QDebug>
#ifdef QT_DEBUG
#include "tests/globaltest.h"
#endif

#include "UpdatesChecker.h"

QLocale wlocale;
bool debugMode = false;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFontDatabase::addApplicationFont(Const::ApplicationDefaultFontPath);
    QApplication::setFont(QFont("Share Tech Mono", 9));

#ifdef QT_DEBUG
    debugMode = true;
    setlocale(LC_CTYPE, "rus");
#endif

//    UpdatesChecker checker;
//    QObject::connect(&checker, &UpdatesChecker::updatesAvailable, [&checker](bool available){
//        if(available) {
//            qDebug() << "UPDATES AVAILABLE" << checker.sizeFormated() << checker.version();
//            checker.startUpdate(qApp);
//        }
//        else
//            qDebug() << "NO UPDATES AVAILABLE";
//    });
//    checker.checkForUpdates();

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

    MainWindow w;
    w.setFont(Const::ApplicationDefaultFontPath);
    w.show();

    DataSource dataSource;

    GuiFactory guiFactory("device_gui.xml");
//    MainViewFacade mvCntrl(dataSource, settings, guiFactory);
//    mvCntrl.addView(&w);

    DeviceFactory deviceFactory("device_db.xml");

    // Фасад для управления потоками данных от модели к представлению
    MainFacade mainFacade(w, guiFactory);

    ModbusProtocol modbus;
    // Модель данных (подключается к источнику данных и забирает их)
    NetworkModel model(deviceFactory, modbus, mainFacade);
//    model.setDelay(100);
    model.setTimeout(300);

    // Контроллер главного окна, управляет потоком данных от GUI к моделии
    MainController mainCtrl(w, model);

#ifdef QT_DEBUG
    GlobalTest tests(argc, argv);
#endif

    return app.exec();
}