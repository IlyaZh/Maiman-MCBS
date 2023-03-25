#pragma once

#include <QDate>
#include <QDir>
#include <QFont>
#include <QPoint>
#include <QString>
#include <array>

bool isEqual(double first, double second);

namespace Const {

enum class PortType { kNone, kTCP, kCom };

// Название организации
static const QString kOrgName{"MaimanElectronics"};
// Название (заголовок) ПО для служебных путей (без пробелов)
static const QString kAppName{"BenchSoft_Multichannel"};
// Человекопонятное название (заголовок_ ПО
static const QString kAppNameTitle{"Maiman BenchSoft Multichannel"};

static const std::array<QString, 2> kTemperatureUnitNames{"Celsius",
                                                          "Fahrenheit"};
enum class TemperatureUnitId { kCelsius, kFahrenheit };

static const struct kNetworkDelayMSecs {
  static const int defaultValue{300};
  static const int min{10};
  static const int max{1000};
} kNetworkDelayMSecs;

static const struct kNetworkTimeoutMSecs {
  static const int defaultValue{500};
  static const int min{10};
  static const int max{1000};
} kNetworkTimeoutMSecs;

static const struct kNetworkMaxAddress {
  static const int defaultValue{32};
  static const int min{1};
  static const int max{63};
} kNetworkMaxAddress;

static const int kNetworkDisconnectedTimeout{100};
static const QString kSecretCodeCombination{"3.1415926"};
static const QString kApplicationDefaultFontPath{
    ":/resources/fonts/ShareTechMono-Regular.ttf"};
static const QFont kApplicationDefaultFont{"Share Tech Mono", 9};
// static const QString kDeviceConfigFile{QDir::currentPath() +
// "/DDBconfig.xml"};
// static const QString kLogTimestampDefaultFormat{"dd.MM.yyyy HH:mm:ss"};
// static const int kLogMaxQueueSize{10};
// static const int kConsoleMaxLinesToShow{1000};
// static const int kMaxFilesInMenu{5};
static const int kStatusbarMessageTimeout{10000};

//    static const QString TemperatureUnitDefault;

static const std::array<QString, 3> kBaudRates{"9600", "115200", "230400"};
static const int kDefaultBaudRate{115200};

static const double kDoubleEpsilon{0.000001};

static const QPoint kWindowDefaultPosition{-1, -1};

// static const QString DEVICE_STATUS_COMMAND{"0004"};
// static const QString TEC_STATUS_COMMAND{"0A1A"};

//// Длительность "покраснения" поля ввода параметров при неверном значении
// static const int CURR_VALUE_BG_ERROR_TIMEOUT{1000};

static const unsigned int kComPortTimeout{100};

static const quint16 kComCommandSendDelay{150};

static const QString kDefaultLogFilename{
    "consoleLog " + QDate::currentDate().toString("dd.MM.yyyy") + ".log"};
static const QString kDefaultLogFilepath{QDir::currentPath() + "/logs/"};

}  // namespace Const
