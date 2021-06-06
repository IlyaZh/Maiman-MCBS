#include "mainwindow.h"
#include "interfaces/mainwindowcontrollerinterface.h"

#include <QApplication>
#include "network/networkmodel.h"
#include "network/protocols/modbus.h"
#include "mainviewfacade.h"
#include "model/devicefactory.h"
#include <QTcpSocket>
#include "globals.h"
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "factories/Parser.h"
#include "factories/xmlparser.h"
#include <QSharedPointer>

#include "factories/xmlparser.h"

#include <QDebug>

QLocale wlocale;
bool debugMode = false;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationVersion(QString::number(MAJOR_VERSION) + "." + QString::number(MINOR_VERSION) + "." + QString::number(PATCH_VERSION));
    QApplication::setFont(APPLICATION_DEFAULT_FONT);

    QSharedPointer<AppSettings> settings = QSharedPointer<AppSettings>(new AppSettings());

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
    w.setFont(APPLICATION_DEFAULT_FONT);
    w.show();

    MainViewFacade* mvCntrl = new MainViewFacade(/*new GUIfactory(new Parser(""), */settings/*)*/);

    mvCntrl->addView(&w);

    DeviceFactory* deviceFactory = new DeviceFactory(new XmlParser("DeviceDB.xml"), settings);
    NetworkModel* model = new NetworkModel(deviceFactory, new Modbus("modbus"));
    model->addFacade(mvCntrl);

    XmlParser *parser = new XmlParser("DeviceDB.xml");
    parser->process();

    return app.exec();
}
