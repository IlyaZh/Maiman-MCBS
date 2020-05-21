#include "appsettings.h"
#include "globals.h"

AppSettings::AppSettings(QObject *parent) : QObject(parent)
{
    settings = new QSettings(QSettings::NativeFormat, QSettings::UserScope, ORG_NAME, APP_NAME);
}

quint32 AppSettings::getComBaudrate() { return settings->value("userSettings/comPort/baudRate", DEFAULT_BAUD_RATE).toUInt(); }
QString AppSettings::getComPort() { return settings->value("userSettings/comPort/port", getDefaultPort()).toString(); }
bool AppSettings::getComAutoconnectFlag() { return settings->value("userSettings/comPort/autoConnect", false).toBool(); }
QString AppSettings::getTemperatureSymbol() { return settings->value("userSettings/temperatureSymbol", DEFAULT_TEMPERATURE_SYMBOL).toString(); }
const QList<QVariant> AppSettings::getRecentOpenFiles() { return settings->value("lastOpenedFiles").toList(); }
QString AppSettings::getLastSaveDirectory() { return settings->value("lastUsedDirectory", QDir::homePath()).toString(); }
uint AppSettings::getComCommandsDelay() { return settings->value("userSettings/comPort/commandsDelay", COM_COMMAND_SEND_DELAY).toUInt(); }
QPoint AppSettings::getWindowPosition() { return settings->value("window/position", WINDOW_DEFAULT_POSITION).toPoint(); }
int AppSettings::getComStopBits() { return settings->value("userSettings/comPort/stopBits", 1).toInt(); }

// slots

void AppSettings::setComBaudrate(quint32 BR) { settings->setValue("userSettings/comPort/baudRate", BR); }

void AppSettings::setComPort(QString port) { settings->setValue("userSettings/comPort/port", port); }

void AppSettings::setComAutoconnectFlag(bool flag) { settings->setValue("userSettings/comPort/autoConnect", flag); }

void AppSettings::setComCommandsDelay(uint delayMs) { settings->setValue("userSettings/comPort/commandsDelay", delayMs); }

void AppSettings::setTemperatureSymbol(QString tempSymbol) { settings->setValue("userSettings/temperatureSymbol", tempSymbol); }

void AppSettings::setRecentOpenFiles(const QList<QVariant> &list) { settings->setValue("lastOpenedFiles", list); }

void AppSettings::setLastSaveDirectory(QString dir) { settings->setValue("lastUsedDirectory", dir); }

void AppSettings::setLastSelectedDeviceId(quint32 id) { settings->setValue("userSettings/lastSelectedDeviceId", id); }

void AppSettings::removeRecentOpenFiles(QString str) { settings->beginGroup("lastOpenedFiles"); settings->remove(str); settings->endGroup(); }

void AppSettings::setWindowPosition(QPoint pos) { settings->setValue("window/position", pos); }

void AppSettings::setComStopBits(int value) { settings->setValue("userSettings/comPort/stopBits", value); }
