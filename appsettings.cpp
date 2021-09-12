#include "appsettings.h"
#include "globals.h"
#include <QJsonDocument>
#include <QDebug>

AppSettings::AppSettings(QObject *parent) :
    QObject(parent),
    settings(new QSettings(QSettings::NativeFormat, QSettings::UserScope, Constants::OrgName, Constants::AppName))
{
}

bool AppSettings::parseFileSettings(QString fileName) {
    QScopedPointer<QFile> file(new QFile(fileName));
    if(!file->exists()) {
        m_errorString = QString("File \"%1\" isn't exist!").arg(fileName);
        return false;
    }
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_errorString = QString("Can't open the file. \"%1\"").arg(fileName);
        return false;
    }
    // Считываем файл настроек и передаем в главное окно

    QJsonParseError jParseErrorObj;
    QJsonDocument jDoc = QJsonDocument::fromJson(file->readAll(), &jParseErrorObj);
    if(jDoc.isNull()) {
        qDebug() << "[E][AppSettings] Can't parse settings file:" << jParseErrorObj.errorString();
    } else {
        // TODO: do it
    }
}

quint32 AppSettings::getComBaudrate() { return settings->value("userSettings/comPort/baudRate", Constants::BaudRateDefault).toUInt(); }
QString AppSettings::getComPort() { return settings->value("userSettings/comPort/port", "").toString(); }
bool AppSettings::getComAutoconnectFlag() { return settings->value("userSettings/comPort/autoConnect", false).toBool(); }
QString AppSettings::getTemperatureSymbol() { return settings->value("userSettings/temperatureSymbol", Constants::TemperatureUnitDefault).toString(); }
const QList<QVariant> AppSettings::getRecentOpenFiles() { return settings->value("lastOpenedFiles").toList(); }
QString AppSettings::getLastSaveDirectory() { return settings->value("lastUsedDirectory", QDir::homePath()).toString(); }
uint AppSettings::getComCommandsDelay() { return settings->value("userSettings/comPort/commandsDelay", COM_COMMAND_SEND_DELAY).toUInt(); }
QPoint AppSettings::getWindowPosition() { return settings->value("window/position", WINDOW_DEFAULT_POSITION).toPoint(); }
int AppSettings::getComStopBits() { return settings->value("userSettings/comPort/stopBits", 1).toInt(); }

NetworkData_s AppSettings::getNetworkData() {
    NetworkData_s netData;
    // TODO: refactoring there
    netData.type = static_cast<NetworkType>(settings->value("network/type", 0).toUInt());
    netData.host = settings->value("network/host", "").toString();
    netData.port = settings->value("network/port", 0).toInt();

    return netData;
}

uint AppSettings::getDeviceTimeout() {
    return settings->value("deviceDefaultTimeout", 1000).toUInt();
}

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

void AppSettings::setNetworkData(/*NetworkData_s*/QVariant netData) {
    /*settings->setValue("network/type", static_cast<uint>(netData.type));
    settings->setValue("network/host", netData.host);
    settings->setValue("network/port", netData.port);*/
    settings->setValue("network", netData);
}

void AppSettings::setDeviceTimeout(quint16 timeoutMs) {
    settings->setValue("deviceDefaultTimeout", timeoutMs);
}
