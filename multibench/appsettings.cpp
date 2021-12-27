#include "appsettings.h"
#include "constants.h"
#include <QJsonDocument>
#include <QDebug>

QScopedPointer<QSettings> AppSettings::settings(new QSettings(QSettings::NativeFormat, QSettings::UserScope, Const::OrgName, Const::AppName));

/*bool AppSettings::parseFileSettings(QString fileName) {
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

}
}*/

    quint32 AppSettings::getComBaudrate() { return settings->value("userSettings/comPort/baudRate", Const::BaudRateDefault).toUInt(); }

QString AppSettings::getComPort() { return settings->value("userSettings/comPort/port", "").toString(); }

bool AppSettings::getComAutoconnectFlag() { return settings->value("userSettings/comPort/autoConnect", false).toBool(); }

Const::TemperatureUnitId AppSettings::getTemperatureUnit() {
    int tempId = settings->value("userSettings/temperatureUnit", static_cast<int>(Const::TemperatureUnitId::Celsius)).toInt();
    return static_cast<Const::TemperatureUnitId>(tempId);
}

const QList<QVariant> AppSettings::getRecentOpenFiles() { return settings->value("lastOpenedFiles").toList(); }

QString AppSettings::getLastSaveDirectory() { return settings->value("lastUsedDirectory", QDir::homePath()).toString(); }

uint AppSettings::getComCommandsDelay() { return settings->value("userSettings/comPort/commandsDelay", Const::COM_COMMAND_SEND_DELAY).toUInt(); }

QPoint AppSettings::getWindowPosition() { return settings->value("window/position", Const::WINDOW_DEFAULT_POSITION).toPoint(); }

int AppSettings::getComStopBits() { return settings->value("userSettings/comPort/stopBits", 1).toInt(); }

NetworkData_s AppSettings::getNetworkData() {
    NetworkData_s netData;
    netData.type = static_cast<NetworkType>(settings->value("network/type", 0).toUInt());
    netData.host = settings->value("network/host", "").toString();
    netData.port = settings->value("network/port", 0).toInt();

    return netData;
}

uint AppSettings::getNetworkTimeout() {
    return settings->value("networkTimeout", Const::NetworkTimeoutMSecs).toUInt();
}

// slots

void AppSettings::setComBaudrate(quint32 BR) { settings->setValue("userSettings/comPort/baudRate", BR); }

void AppSettings::setComPort(QString port) { settings->setValue("userSettings/comPort/port", port); }

void AppSettings::setComAutoconnectFlag(bool flag) { settings->setValue("userSettings/comPort/autoConnect", flag); }

void AppSettings::setComCommandsDelay(uint delayMs) { settings->setValue("userSettings/comPort/commandsDelay", delayMs); }

void AppSettings::setTemperatureUnit(const QString& unit) {
    Const::TemperatureUnitId id = (unit == "Celsius") ? Const::TemperatureUnitId::Celsius : Const::TemperatureUnitId::Fahrenheit;
    settings->setValue("userSettings/temperatureUnit", static_cast<int>(id));
}

void AppSettings::setTemperatureUnit(Const::TemperatureUnitId id) {
    settings->setValue("userSettings/temperatureUnit", static_cast<int>(id));
}

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

void AppSettings::setNetworkTimeout(quint16 timeoutMs) {
    settings->setValue("networkTimeout", timeoutMs);
}

void AppSettings::setDeviceAddresses(const QMap<quint8,quint8>& addr){
    settings->remove("userSettings/addresses");
    settings->beginWriteArray("userSettings/addresses");
    int i = 0;
    for(const auto& item : addr.keys()){
        settings->setArrayIndex(i++);
        settings->setValue("userSettings/addresses", item);
    }
    settings->endArray();
}

QMap<quint8,quint8> AppSettings::getDeviceAddresses(){
    QMap<quint8,quint8> map;
    int size = settings->beginReadArray("userSettings/addresses");
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        map.insert(settings->value("userSettings/addresses").toUInt(),0);
    }
    settings->endArray();
    return map;
}

void AppSettings::setKeepAddresses(bool flag){
    settings->setValue("userSettings/keepAddresses", flag);
}

bool AppSettings::getKeepAddresses(){
     return settings->value("userSettings/keepAddresses", false).toBool();
}
