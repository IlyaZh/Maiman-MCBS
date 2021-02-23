#include <QString>
#include <QStringList>
#include <QMap>
#include <QDate>
#include <QDir>
#include <QSerialPortInfo>
#include <QPointer>
#include <QFont>
#include <QPoint>
//#include "appsettings.h"

#ifndef GLOBALS_H
#define GLOBALS_H

// Название организации
const QString ORG_NAME = "MaimanElectronics";
// Название (заголовок) ПО для служебных путей (без пробелов)
const QString APP_NAME = "BenchSoft_Multichannel";
// Человекопонятное название (заголовок_ ПО
const QString APP_NAME_TITLE = "Maiman BenchSoft Multichannel";
const QPoint WINDOW_DEFAULT_POSITION = QPoint(-1, -1);
//const QSize WINDOW_DEFAULT_SIZE = QSize(-1,-1);

const QString DEVICE_STATUS_COMMAND = "0700";
const QString TEC_STATUS_COMMAND = "0A1A";
const QString FREQUENCY_COMMAND = "0100";
const QString DURATION_COMMAND = "0200";

const quint8 MAJOR_VERSION = 1;
const quint8 MINOR_VERSION = 0;
const quint8 PATCH_VERSION = 0;

const quint8 MAX_DEVICES = 32;

const QString SECRET_CODE_COMBINATION = "3.1415926";

const QFont APPLICATION_DEFAULT_FONT(":/fonts/ShareTechMono.ttf", 9);

const QString CONFIG_FILE = QDir::currentPath()+"/DDBconfig.xml";
const QString LOG_TIMESTAMP_DEFAULT_FORMAT = "dd.MM.yyyy HH:mm:ss";
const int LOG_MAX_QUEUE_SIZE = 10;
const unsigned int CONSOLE_MAX_LINES_TO_SHOW = 1000;
const int MAX_FILES_IN_MENU = 5;
const int STATUSBAR_MESSAGE_TIMEOUT = 5000; // ms

// Длительность "покраснения" поля ввода параметров при неверном значении
const int CURR_VALUE_BG_ERROR_TIMEOUT = 1000; // ms
const quint16 IDENTIFY_DEVICE_COMMAND = 0x0702;

const unsigned int DEFAULT_BAUD_RATE = 115200;

const unsigned int COM_PORT_TIMEOUT = 1000; // [ms]

const QString DEFAULT_TEMPERATURE_SYMBOL = "C";

const unsigned int COM_COMMAND_SEND_DELAY = 150; // ms
const uint COM_COMMAND_MIN_SEND_DELAY = 50;
const uint COM_COMMAND_MAX_SEND_DELAY = 1000;

const unsigned int STOP_COMMAND_DELAY_DEFAULT = COM_PORT_TIMEOUT-100; // [ms]

const QString DEFAULT_LOG_FILENAME = "consoleLog " + QDate::currentDate().toString("dd.MM.yyyy") + ".log";
const QString DEFAULT_LOG_FILEPATH = QDir::currentPath()+"/logs/";

const int DEFAULT_DIGITS_AFTER_POINT = 2;
#define DEFAULT_FORMAT_FOR_PARAMETERS 'f', DEFAULT_DIGITS_AFTER_POINT
const char DOUBLE_FORMAT = 'f';

inline const QStringList getAvailablePorts() {
    QStringList ports;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
       ports.append(info.portName());
    }
    return ports;
}
inline QString getDefaultPort() { return getAvailablePorts().value(0); }

enum deviceStatusMask {START_STOP_MASK = 0x2, CURRENT_EXT_INT_MASK = 0x4, START_EXT_INT_MASK = 0x10, BLOCK_THERMO_MASK = 0x40, BLOCK_USE_IGNORE_MASK = 0x80};
const QString appTitle = QString("%1 v.%2.%3.%4").arg(APP_NAME_TITLE).arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(PATCH_VERSION);


#endif // GLOBALS_H
