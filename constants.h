#pragma once

//#include "appsettings.h"
#include <QString>
#include <QPoint>

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

    static const QPoint WINDOW_DEFAULT_POSITION;
    //static const QSize WINDOW_DEFAULT_SIZE = QSize(-1,-1);

    static const QString DEVICE_STATUS_COMMAND;
    static const QString TEC_STATUS_COMMAND;
    static const QString FREQUENCY_COMMAND;
    static const QString DURATION_COMMAND;

    // Длительность "покраснения" поля ввода параметров при неверном значении
    static const int CURR_VALUE_BG_ERROR_TIMEOUT;

    static const unsigned int COM_PORT_TIMEOUT;

    static const quint16 COM_COMMAND_SEND_DELAY;

    static const QString DEFAULT_LOG_FILENAME;
    static const QString DEFAULT_LOG_FILEPATH;
};

//static const int DEFAULT_DIGITS_AFTER_POINT = 2;
//#define DEFAULT_FORMAT_FOR_PARAMETERS 'f', DEFAULT_DIGITS_AFTER_POINT
//static const char DOUBLE_FORMAT = 'f';

//enum deviceStatusMask {START_STOP_MASK = 0x2, CURRENT_EXT_INT_MASK = 0x4, START_EXT_INT_MASK = 0x10, BLOCK_THERMO_MASK = 0x40, BLOCK_USE_IGNORE_MASK = 0x80};