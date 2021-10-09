#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QPoint>
#include <QSize>
#include <QDir>
#include "constants.h"
#include "datasource.h"
#include <QScopedPointer>

typedef struct NetworkData_s {
    NetworkType type;
    QString host;
    int port;
} NetworkData_s;

class AppSettings : public QObject
{
    Q_OBJECT
public:
    explicit AppSettings(QObject *parent = nullptr);
    bool parseFileSettings(QString fileName);
    QString errorString();
    quint32 getComBaudrate();
    QString getComPort();
    uint getComAutoconnectFlag(bool flag);
    uint getComCommandsDelay();
    bool getComAutoconnectFlag();
    QString getTemperatureSymbol();
    const QList<QVariant> getRecentOpenFiles();
    QString getLastSaveDirectory();
    QPoint getWindowPosition();
    int getComStopBits();
    NetworkData_s getNetworkData();
    uint getDeviceTimeout();
//    QSize getWindowSize();

private:
    QSettings* settings;
    QString m_errorString;

signals:

public slots:
    void setComBaudrate(quint32 BR);
    void setComPort(QString port);
    void setComAutoconnectFlag(bool flag);
    void setComCommandsDelay(uint delayMs);
    void setTemperatureSymbol(QString tempSymbol);
    void setRecentOpenFiles(const QList<QVariant> &list);
    void setLastSaveDirectory(QString dir = QDir::homePath());
    void setLastSelectedDeviceId(quint32 id);
    void removeRecentOpenFiles(QString str);
    void setWindowPosition(QPoint);
    void setComStopBits(int);
    void setNetworkData(/*NetworkData_s*/QVariant netData);
    void setDeviceTimeout(quint16 timeoutMs);
//    void setWindowSize(QSize);
};

#endif // APPSETTINGS_H
