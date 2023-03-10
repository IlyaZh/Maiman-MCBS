#include "constants.h"

#include <QStringList>
#include <QMap>
#include <QDate>
#include <QDir>
#include <QPointer>
#include <QFont>
#include <QCoreApplication>

// Название организации
const QString Const::OrgName = "MaimanElectronics";
// Название (заголовок) ПО для служебных путей (без пробелов)
const QString Const::AppName = "BenchSoft_Multichannel";
// Человекопонятное название (заголовок_ ПО
const QString Const::AppNameTitle = "Maiman BenchSoft Multichannel";

const QStringList Const::TemperatureUnitNames({"Celsius", "Fahrenheit"});

const int Const::NetworkDelayMSecs::defaultValue = 300;
const int Const::NetworkDelayMSecs::min = 10;
const int Const::NetworkDelayMSecs::max = 1000;

const int Const::NetworkTimeoutMSecs::defaultValue = 500;
const int Const::NetworkTimeoutMSecs::min = 10;
const int Const::NetworkTimeoutMSecs::max = 1000;

const int Const::NetworkMaxAddress::defaultValue = 32;
const int Const::NetworkMaxAddress::min = 1;
const int Const::NetworkMaxAddress::max = 64;

const QString Const::SecretCodeCombination = "3.1415926";

const QString Const::ApplicationDefaultFontPath = ":/resources/fonts/ShareTechMono-Regular.ttf";

const QFont Const::ApplicationDefaultFont("Share Tech Mono", 9);

const QString Const::DeviceConfigFile = QDir::currentPath()+"/DDBconfig.xml";

const QString Const::LogTimestampDefaultFormat = "dd.MM.yyyy HH:mm:ss";

const int Const::LogMaxQueueSize = 10;

const int Const::ConsoleMaxLinesToShow = 1000;

const int Const::MaxFilesInMenu = 5;

const int Const::StatusbarMessageTimeout = 10000; // ms

const QStringList Const::BaudRates({"9600", "115200","230400"});

const int Const::BaudRateDefault = 115200;

const double Const::DoubleEpsilon = 0.000001;

const QPoint Const::WINDOW_DEFAULT_POSITION = QPoint(-1, -1);

const QString Const::DEVICE_STATUS_COMMAND = "0004";
const QString Const::TEC_STATUS_COMMAND = "0A1A";
const QString Const::FREQUENCY_COMMAND = "0100";
const QString Const::DURATION_COMMAND = "0200";



// Длительность "покраснения" поля ввода параметров при неверном значении
const int Const::CURR_VALUE_BG_ERROR_TIMEOUT = 1000; // ms

const unsigned int Const::ComPortTimeout = 100; // [ms]

const quint16 Const::COM_COMMAND_SEND_DELAY = 150; // ms

const QString Const::DEFAULT_LOG_FILENAME("consoleLog " + QDate::currentDate().toString("dd.MM.yyyy") + ".log");
const QString Const::DEFAULT_LOG_FILEPATH(QDir::currentPath()+"/logs/");
