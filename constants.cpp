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

const quint8 Const::MaxDevices = 32;

const QString Const::SecretCodeCombination = "3.1415926";

const QString Const::DeviceConfigFile = QDir::currentPath()+"/DDBconfig.xml";

const QString Const::LogTimestampDefaultFormat = "dd.MM.yyyy HH:mm:ss";

const int Const::LogMaxQueueSize = 10;

const int Const::ConsoleMaxLinesToShow = 1000;

const int Const::MaxFilesInMenu = 5;

const int Const::StatusbarMessageTimeout = 5000; // ms

const quint16 Const::IdentifyDeviceCommand = 0x0001;

const QString Const::TemperatureUnitDefault = "C";

const QStringList Const::BaudRates({"9600", "115200"});

const int Const::BaudRateDefault = 115200;

const double Const::DoubleEpsilon = 0.000001;

const QPoint Const::WINDOW_DEFAULT_POSITION = QPoint(-1, -1);
//const QSize Const::WINDOW_DEFAULT_SIZE = QSize(-1,-1);

const QString Const::DEVICE_STATUS_COMMAND = "0004";
const QString Const::TEC_STATUS_COMMAND = "0A1A";
const QString Const::FREQUENCY_COMMAND = "0100";
const QString Const::DURATION_COMMAND = "0200";



// Длительность "покраснения" поля ввода параметров при неверном значении
const int Const::CURR_VALUE_BG_ERROR_TIMEOUT = 1000; // ms

const unsigned int Const::COM_PORT_TIMEOUT = 100; // [ms]

const quint16 Const::COM_COMMAND_SEND_DELAY = 150; // ms

const QString Const::DEFAULT_LOG_FILENAME = "consoleLog " + QDate::currentDate().toString("dd.MM.yyyy") + ".log";
const QString Const::DEFAULT_LOG_FILEPATH = QDir::currentPath()+"/logs/";
