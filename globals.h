#include <QString>
#include <QStringList>
#include <QMap>
#include <QDate>
#include <QDir>
#include <QPointer>
#include <QFont>
#include <QPoint>
#include <QStringView>
//#include "appsettings.h"

#ifndef GLOBALS_H
#define GLOBALS_H

namespace Constants {
    // Название организации
    const QString OrgName = "MaimanElectronics";
    // Название (заголовок) ПО для служебных путей (без пробелов)
    const QString AppName = "BenchSoft_Multichannel";
    // Человекопонятное название (заголовок_ ПО
    const QString AppNameTitle = "Maiman BenchSoft Multichannel";

    const quint8 MajorVersion = 1;
    const quint8 MinorVersion = 0;
    const quint8 PatchVersion = 0;
    const QString AppTitle = QString("%1 v.%2.%3.%4").arg(Constants::AppNameTitle).arg(Constants::MajorVersion).arg(Constants::MinorVersion).arg(Constants::PatchVersion);

    const quint8 MaxDevices = 32;
    const QString SecretCodeCombination = "3.1415926";
    const QFont ApplicationDefaultFont(":/fonts/ShareTechMono.ttf", 9);
    const QString DeviceConfigFile = QDir::currentPath()+"/DDBconfig.xml";
    const QString LogTimestampDefaultFormat = "dd.MM.yyyy HH:mm:ss";
    const int LogMaxQueueSize = 10;
    const unsigned int ConsoleMaxLinesToShow = 1000;
    const int MaxFilesInMenu = 5;
    const int StatusbarMessageTimeout = 5000; // ms

    const quint16 IdentifyDeviceCommand = 0x0001;
    const QString TemperatureUnitDefault = "C";

    const unsigned int BaudRateDefault = 115200;
};

const QPoint WINDOW_DEFAULT_POSITION = QPoint(-1, -1);
//const QSize WINDOW_DEFAULT_SIZE = QSize(-1,-1);

const QString DEVICE_STATUS_COMMAND = "0004";
const QString TEC_STATUS_COMMAND = "0A1A";
const QString FREQUENCY_COMMAND = "0100";
const QString DURATION_COMMAND = "0200";



// Длительность "покраснения" поля ввода параметров при неверном значении
const int CURR_VALUE_BG_ERROR_TIMEOUT = 1000; // ms




const unsigned int COM_PORT_TIMEOUT = 100; // [ms]



const quint16 COM_COMMAND_SEND_DELAY = 150; // ms


const QString DEFAULT_LOG_FILENAME = "consoleLog " + QDate::currentDate().toString("dd.MM.yyyy") + ".log";
const QString DEFAULT_LOG_FILEPATH = QDir::currentPath()+"/logs/";

//const int DEFAULT_DIGITS_AFTER_POINT = 2;
//#define DEFAULT_FORMAT_FOR_PARAMETERS 'f', DEFAULT_DIGITS_AFTER_POINT
//const char DOUBLE_FORMAT = 'f';

//enum deviceStatusMask {START_STOP_MASK = 0x2, CURRENT_EXT_INT_MASK = 0x4, START_EXT_INT_MASK = 0x10, BLOCK_THERMO_MASK = 0x40, BLOCK_USE_IGNORE_MASK = 0x80};


#endif // GLOBALS_H
