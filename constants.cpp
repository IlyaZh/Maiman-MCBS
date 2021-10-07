#include "constants.h"

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
