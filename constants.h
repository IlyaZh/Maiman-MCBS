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

#pragma once

bool isEqual(double first, double second);

class Const {
public:
    // Название организации
    static const QString OrgName;
    // Название (заголовок) ПО для служебных путей (без пробелов)
    static const QString AppName;
    // Человекопонятное название (заголовок_ ПО
    static const QString AppNameTitle;

    static const quint8 MaxDevices;
    static const QString SecretCodeCombination;
//    static const QFont ApplicationDefaultFont("qrc://resources/fonts/ShareTechMono-Regular.ttf", 9);
    static const QString DeviceConfigFile;
    static const QString LogTimestampDefaultFormat;
    static const int LogMaxQueueSize;
    static const int ConsoleMaxLinesToShow;
    static const int MaxFilesInMenu;
    static const int StatusbarMessageTimeout;

    static const quint16 IdentifyDeviceCommand;
    static const QString TemperatureUnitDefault;

    static const QStringList BaudRates;
    static const int BaudRateDefault;

    static const double DoubleEpsilon;

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
