#include <QString>
#include <QStringList>
#include <QMap>
#include <QDate>
#include <QDir>
#include <QPointer>
#include <QFont>
#include <QPoint>
#include <QStringView>
#include <QCoreApplication>
//#include "appsettings.h"

#ifndef GLOBALS_H
#define GLOBALS_H

namespace Constants {
    // Название организации
    static const QString OrgName = "MaimanElectronics";
    // Название (заголовок) ПО для служебных путей (без пробелов)
    static const QString AppName = "BenchSoft_Multichannel";
    // Человекопонятное название (заголовок_ ПО
    static const QString AppNameTitle = "Maiman BenchSoft Multichannel";

    static const QString AppTitle = QString("%1 v.%2").arg(Constants::AppNameTitle, QCoreApplication::applicationVersion());

    static const quint8 MaxDevices = 32;
    static const QString SecretCodeCombination = "3.1415926";
    static const QFont ApplicationDefaultFont("://resources/fonts/ShareTechMono-Regular.ttf", 9);
    static const QString DeviceConfigFile = QDir::currentPath()+"/DDBconfig.xml";
    static const QString LogTimestampDefaultFormat = "dd.MM.yyyy HH:mm:ss";
    static const int LogMaxQueueSize = 10;
    static const unsigned int ConsoleMaxLinesToShow = 1000;
    static const int MaxFilesInMenu = 5;
    static const int StatusbarMessageTimeout = 5000; // ms

    static const quint16 IdentifyDeviceCommand = 0x0001;
    static const QString TemperatureUnitDefault = "C";

    static const unsigned int BaudRateDefault = 115200;
};

static const QPoint WINDOW_DEFAULT_POSITION = QPoint(-1, -1);
//static const QSize WINDOW_DEFAULT_SIZE = QSize(-1,-1);

static const QString DEVICE_STATUS_COMMAND = "0004";
static const QString TEC_STATUS_COMMAND = "0A1A";
static const QString FREQUENCY_COMMAND = "0100";
static const QString DURATION_COMMAND = "0200";



// Длительность "покраснения" поля ввода параметров при неверном значении
static const int CURR_VALUE_BG_ERROR_TIMEOUT = 1000; // ms




static const unsigned int COM_PORT_TIMEOUT = 100; // [ms]



static const quint16 COM_COMMAND_SEND_DELAY = 150; // ms


static const QString DEFAULT_LOG_FILENAME = "consoleLog " + QDate::currentDate().toString("dd.MM.yyyy") + ".log";
static const QString DEFAULT_LOG_FILEPATH = QDir::currentPath()+"/logs/";

//static const int DEFAULT_DIGITS_AFTER_POINT = 2;
//#define DEFAULT_FORMAT_FOR_PARAMETERS 'f', DEFAULT_DIGITS_AFTER_POINT
//static const char DOUBLE_FORMAT = 'f';

//enum deviceStatusMask {START_STOP_MASK = 0x2, CURRENT_EXT_INT_MASK = 0x4, START_EXT_INT_MASK = 0x10, BLOCK_THERMO_MASK = 0x40, BLOCK_USE_IGNORE_MASK = 0x80};


#endif // GLOBALS_H
