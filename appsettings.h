#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QPoint>
#include <QSize>
#include <QDir>

class AppSettings : public QObject
{
    Q_OBJECT
public:
    explicit AppSettings(QObject *parent = nullptr);
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
//    QSize getWindowSize();

private:
    QSettings *settings;

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
//    void setWindowSize(QSize);
};

#endif // APPSETTINGS_H
