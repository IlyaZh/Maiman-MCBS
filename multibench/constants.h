#pragma once

#include <QString>
#include <QPoint>
#include <QFont>

bool isEqual(double first, double second);

enum class PortType {
    None,
    TCP,
    Com
};

struct Const {
    // Название организации
    static const QString OrgName;
    // Название (заголовок) ПО для служебных путей (без пробелов)
    static const QString AppName;
    // Человекопонятное название (заголовок_ ПО
    static const QString AppNameTitle;

    static const QStringList TemperatureUnitNames;
    enum class TemperatureUnitId {
        Celsius,
        Fahrenheit
    };

    struct NetworkTimeoutMSecs {
        static const int defaultValue;
        static const int min;
        static const int max;
    };

    static const int NetworkDelayMSecs;

    static const QString SecretCodeCombination;
    static const QString ApplicationDefaultFontPath;
    static const QFont ApplicationDefaultFont;
    static const QString DeviceConfigFile;
    static const QString LogTimestampDefaultFormat;
    static const int LogMaxQueueSize;
    static const int ConsoleMaxLinesToShow;
    static const int MaxFilesInMenu;
    static const int StatusbarMessageTimeout;

//    static const QString TemperatureUnitDefault;

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

    static const unsigned int ComPortTimeout;

    static const quint16 COM_COMMAND_SEND_DELAY;

    static const QString DEFAULT_LOG_FILENAME;
    static const QString DEFAULT_LOG_FILEPATH;
};
