#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QFontDatabase>
#include <QLocale>
#include <QTcpSocket>

#include "components/mediator.h"
#include "constants.h"
#include "gui/guimediator.h"
#include "mainwindow.h"
#include "model/devicefactory.h"
#include "model/guifactory.h"
#include "modelguimediator.h"
#include "network/networkmodel.h"
#include "network/protocols/modbusprotocol.h"
#ifdef QT_DEBUG
#include <locale.h>
#endif

QLocale wlocale;
bool debugMode = false;
void messageToFile(QtMsgType type, const QMessageLogContext &context,
                   const QString &msg);

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QFontDatabase::addApplicationFont(Const::kApplicationDefaultFontPath);
  QApplication::setFont(QFont("Share Tech Mono", 9));

#ifdef QT_DEBUG
  debugMode = false;
  setlocale(LC_CTYPE, "ru-RU");
#endif

  QCommandLineParser cliParser;
  QCommandLineOption debugOption(QStringList() << "d"
                                               << "debug");
  cliParser.addOption(debugOption);
  cliParser.parse(QCoreApplication::arguments());
  if (cliParser.isSet(debugOption)) {
    debugMode = true;
  }
  QLocale::setDefault(QLocale::English);
  wlocale = QLocale(QLocale::system());

  MainWindow w;
  w.setFont(Const::kApplicationDefaultFontPath);
  w.show();

  GuiFactory guiFactory("device_gui.xml");

  DeviceFactory deviceFactory("device_db.xml");

  ModbusProtocol modbus;
  NetworkModel model(deviceFactory, modbus);
  components::Mediator mediator;
  ModelGuiMediator mainMediator(w, guiFactory, model);
  GuiMediator gui(w, guiFactory, model);

  mediator.Subscribe(model::EventType::kDeviceStateUpdated, &model);
  mediator.LinkPublisher(&w);
  mediator.Subscribe(model::EventType::kDeviceStateUpdated, &gui);
  mediator.LinkPublisher(&model);
  mediator.Subscribe(model::EventType::kWriteDevice, &model);
  mediator.LinkPublisher(&gui);

#ifdef QT_DEBUG
  // GlobalTest tests(argc, argv);
#endif
  if (debugMode) {
    qInstallMessageHandler(messageToFile);
  }
  return app.exec();
}

void messageToFile(QtMsgType type, const QMessageLogContext &context,
                   const QString &msg) {
  QFile file(Const::kDefaultLogFilepath + Const::kDefaultLogFilename);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
    return;
  }

  QTextStream out(&file);

  switch (type) {
    case QtInfoMsg:
      out << QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss:zzz")
          << " Info: " << msg << ",     " << context.file << endl;
      break;
    case QtWarningMsg:
      out << QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss:zzz")
          << " Warning: " << msg << ",      " << context.file << endl;
      break;
    case QtCriticalMsg:
      out << QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss:zzz")
          << " Critical: " << msg << ",  " << context.file << endl;
      break;
    case QtFatalMsg:
      out << QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss:zzz")
          << " Fatal: " << msg << ",     " << context.file << endl;
      break;
    default:
      break;
  }
  file.close();
}
