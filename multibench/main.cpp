#include "mainwindow.h"

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
#include <QFontDatabase>
#include "modelguimediator.h"

#include <QDebug>
#ifdef QT_DEBUG
#include <locale.h>
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
    setlocale(LC_CTYPE, "ru-RU");
#endif
  
    QCommandLineParser cliParser;
    QCommandLineOption debugOption(QStringList() << "d" << "debug");
    cliParser.addOption(debugOption);
    cliParser.parse(QCoreApplication::arguments());
    if(cliParser.isSet(debugOption)) {
        debugMode = true;
    }
    QLocale::setDefault(QLocale::English);
    wlocale = QLocale(QLocale::system());

    MainWindow w;
    w.setFont(Const::ApplicationDefaultFontPath);
    w.show();

    // DataSource dataSource;

    GuiFactory guiFactory("/device_gui.xml");

    DeviceFactory deviceFactory("/device_db.xml");


    ModbusProtocol modbus;
    NetworkModel model(deviceFactory, modbus);

    ModelGuiMediator mainMediator(w, guiFactory, model);

#ifdef QT_DEBUG
    //GlobalTest tests(argc, argv);
#endif

    return app.exec();
}
