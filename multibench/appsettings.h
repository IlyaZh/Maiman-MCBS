#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QtCore>

//#include <QObject>
//#include <QSettings>
//#include <QPoint>
//#include <QSize>
//#include <QDir>
#include <QSet>
#include "constants.h"
#include "datasource.h"
//#include <QScopedPointer>

typedef struct NetworkData_s {
    PortType type;
    QString host;
    int port;
} NetworkData_s;

class AppSettings : public QObject
{
    Q_OBJECT
public:
    static bool parseFileSettings(QString fileName);
    static QString errorString();
    static quint32 getComBaudrate();
    static QString getComPort();
    static uint getComAutoconnectFlag(bool flag);
    static uint getComCommandsDelay();
    static bool getComAutoconnectFlag();
    static Const::TemperatureUnitId getTemperatureUnit();
    static const QList<QVariant> getRecentOpenFiles();
    static QString getLastSaveDirectory();
    static QPoint getWindowPosition();
    static int getComStopBits();
    static NetworkData_s getNetworkData();
    static uint getDeviceTimeout();
    static QSet<quint8> getDeviceAddresses();
    static bool getKeepAddresses();
    //    QSize getWindowSize();

private:
    static QScopedPointer<QSettings> settings;
    //    QString m_errorString;

signals:

public slots:
    static void setComBaudrate(quint32 BR);
    static void setComPort(QString port);
    static void setComAutoconnectFlag(bool flag);
    static void setComCommandsDelay(uint delayMs);
    static void setTemperatureUnit(const QString& unit);
    static void setTemperatureUnit(Const::TemperatureUnitId id);
    static void setRecentOpenFiles(const QList<QVariant> &list);
    static void setLastSaveDirectory(QString dir = QDir::homePath());
    static void setLastSelectedDeviceId(quint32 id);
    static void removeRecentOpenFiles(QString str);
    static void setWindowPosition(QPoint);
    static void setComStopBits(int);
    static void setNetworkData(/*NetworkData_s*/QVariant netData);
    static void setDeviceTimeout(quint16 timeoutMs);
    static void setDeviceAddresses(const QSet<quint8>& addr);
    static void setKeepAddresses(bool flag);
    //    static void setWindowSize(QSize);
};

#endif // APPSETTINGS_H
