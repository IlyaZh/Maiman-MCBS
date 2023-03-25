#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSet>
#include <QtCore>

#include "constants.h"

typedef struct NetworkData_s {
  Const::PortType type;
  QString port;
  int baudrate;
} NetworkData_s;

class AppSettings : public QObject {
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
  static int getNetworkDelay();
  static int getNetworkTimeout();
  static int getNetworkMaxAddress();
  static QMap<quint8, quint8> getDeviceAddresses();
  static bool getKeepAddresses();

 private:
  static QScopedPointer<QSettings> settings;

 signals:

 public slots:
  static void setComBaudrate(quint32 BR);
  static void setComPort(QString port);
  static void setComAutoconnectFlag(bool flag);
  static void setComCommandsDelay(uint delayMs);
  static void setTemperatureUnit(const QString& unit);
  static void setTemperatureUnit(Const::TemperatureUnitId id);
  static void setRecentOpenFiles(const QList<QVariant>& list);
  static void setLastSaveDirectory(QString dir = QDir::homePath());
  static void setLastSelectedDeviceId(quint32 id);
  static void removeRecentOpenFiles(QString str);
  static void setWindowPosition(QPoint);
  static void setComStopBits(int);
  static void setNetworkData(QVariant netData);
  static void setNetworkDelay(int delayMs);
  static void setNetworkTimeout(int timeoutMs);
  static void setNetworkMaxAddress(int address);
  static void setDeviceAddresses(const QMap<quint8, quint8>& addr);
  static void setKeepAddresses(bool flag);
};

#endif  // APPSETTINGS_H
