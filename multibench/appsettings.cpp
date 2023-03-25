#include "appsettings.h"

#include <QJsonDocument>

#include "constants.h"

QScopedPointer<QSettings> AppSettings::settings(
    new QSettings(QSettings::NativeFormat, QSettings::UserScope,
                  Const::kOrgName, Const::kAppName));

quint32 AppSettings::getComBaudrate() {
  return settings
      ->value("userSettings/comPort/baudRate", Const::kDefaultBaudRate)
      .toUInt();
}

QString AppSettings::getComPort() {
  return settings->value("userSettings/comPort/port", "").toString();
}

bool AppSettings::getComAutoconnectFlag() {
  return settings->value("userSettings/comPort/autoConnect", false).toBool();
}

Const::TemperatureUnitId AppSettings::getTemperatureUnit() {
  int tempId = settings
                   ->value("userSettings/temperatureUnit",
                           static_cast<int>(Const::TemperatureUnitId::kCelsius))
                   .toInt();
  return static_cast<Const::TemperatureUnitId>(tempId);
}

const QList<QVariant> AppSettings::getRecentOpenFiles() {
  return settings->value("lastOpenedFiles").toList();
}

QString AppSettings::getLastSaveDirectory() {
  return settings->value("lastUsedDirectory", QDir::homePath()).toString();
}

uint AppSettings::getComCommandsDelay() {
  return settings
      ->value("userSettings/comPort/commandsDelay", Const::kComCommandSendDelay)
      .toUInt();
}

QPoint AppSettings::getWindowPosition() {
  return settings->value("window/position", Const::kWindowDefaultPosition)
      .toPoint();
}

int AppSettings::getComStopBits() {
  return settings->value("userSettings/comPort/stopBits", 1).toInt();
}

NetworkData_s AppSettings::getNetworkData() {
  NetworkData_s netData;
  netData.type =
      static_cast<Const::PortType>(settings->value("network/type", 0).toUInt());
  netData.port = settings->value("network/comport", "").toString();
  netData.baudrate = settings->value("network/baudrate", 0).toInt();

  return netData;
}

int AppSettings::getNetworkDelay() {
  return settings
      ->value("networkDelay", Const::kNetworkDelayMSecs::defaultValue)
      .toInt();
}

int AppSettings::getNetworkTimeout() {
  return settings
      ->value("networkTimeout", Const::kNetworkTimeoutMSecs::defaultValue)
      .toInt();
}

int AppSettings::getNetworkMaxAddress() {
  return settings
      ->value("networkMaxAddress", Const::kNetworkMaxAddress::defaultValue)
      .toInt();
}
// slots

void AppSettings::setComBaudrate(quint32 BR) {
  settings->setValue("userSettings/comPort/baudRate", BR);
}

void AppSettings::setComPort(QString port) {
  settings->setValue("userSettings/comPort/port", port);
}

void AppSettings::setComAutoconnectFlag(bool flag) {
  settings->setValue("userSettings/comPort/autoConnect", flag);
}

void AppSettings::setComCommandsDelay(uint delayMs) {
  settings->setValue("userSettings/comPort/commandsDelay", delayMs);
}

void AppSettings::setTemperatureUnit(const QString& unit) {
  Const::TemperatureUnitId id = (unit == "Celsius")
                                    ? Const::TemperatureUnitId::kCelsius
                                    : Const::TemperatureUnitId::kFahrenheit;
  settings->setValue("userSettings/temperatureUnit", static_cast<int>(id));
}

void AppSettings::setTemperatureUnit(Const::TemperatureUnitId id) {
  settings->setValue("userSettings/temperatureUnit", static_cast<int>(id));
}

void AppSettings::setRecentOpenFiles(const QList<QVariant>& list) {
  settings->setValue("lastOpenedFiles", list);
}

void AppSettings::setLastSaveDirectory(QString dir) {
  settings->setValue("lastUsedDirectory", dir);
}

void AppSettings::setLastSelectedDeviceId(quint32 id) {
  settings->setValue("userSettings/lastSelectedDeviceId", id);
}

void AppSettings::removeRecentOpenFiles(QString str) {
  settings->beginGroup("lastOpenedFiles");
  settings->remove(str);
  settings->endGroup();
}

void AppSettings::setWindowPosition(QPoint pos) {
  settings->setValue("window/position", pos);
}

void AppSettings::setComStopBits(int value) {
  settings->setValue("userSettings/comPort/stopBits", value);
}

void AppSettings::setNetworkData(QVariant netData) {
  auto map = netData.toMap();
  settings->setValue("network/comport", map.value("comport"));
  settings->setValue("network/baudrate", map.value("baudrate"));
  settings->setValue("network/type", map.value("type"));
}

void AppSettings::setNetworkDelay(int delayMs) {
  settings->setValue("networkDelay", delayMs);
}

void AppSettings::setNetworkTimeout(int timeoutMs) {
  settings->setValue("networkTimeout", timeoutMs);
}

void AppSettings::setNetworkMaxAddress(int address) {
  settings->setValue("networkMaxAddress", address);
}

void AppSettings::setDeviceAddresses(const QMap<quint8, quint8>& addr) {
  settings->remove("userSettings/addresses");
  settings->beginWriteArray("userSettings/addresses");
  int i = 0;
  for (const auto& item : addr.keys()) {
    settings->setArrayIndex(i++);
    settings->setValue("userSettings/addresses", item);
  }
  settings->endArray();
}

QMap<quint8, quint8> AppSettings::getDeviceAddresses() {
  QMap<quint8, quint8> map;
  int size = settings->beginReadArray("userSettings/addresses");
  for (int i = 0; i < size; ++i) {
    settings->setArrayIndex(i);
    map.insert(settings->value("userSettings/addresses").toUInt(), 0);
  }
  settings->endArray();
  return map;
}

void AppSettings::setKeepAddresses(bool flag) {
  settings->setValue("userSettings/keepAddresses", flag);
}

bool AppSettings::getKeepAddresses() {
  return settings->value("userSettings/keepAddresses", false).toBool();
}
