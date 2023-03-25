#ifndef DEVICEMODELFACTORY_H
#define DEVICEMODELFACTORY_H

#include <QMultiMap>
#include <QObject>
#include <QSharedPointer>
#include <QThread>

#include "device/device.h"
#include "device/devicemodel.h"
#include "factories/parserworker.h"

class DeviceFactory : public QObject {
  Q_OBJECT
 public:
  explicit DeviceFactory(const QString& fileName, QObject* parent = nullptr);
  void start();
  QSharedPointer<Device> createDevice(quint8 addr, quint16 id);
  QStringList getBaudrate();
  const QMultiMap<uint, QString>& getCommonDeviceIDs() const;

 signals:
  void parsingIsFinished();

 private slots:
  void parsingFinished();
  void threadError(const QString&);

 private:
  QString m_fileName;

  const DeviceModel findModel(quint16 id);
  bool parseTree(const TreeItem& tree);
  QString parseBaudRate(const TreeItem& item);
  QMultiMap<uint, QString> parseCommonDevId(const TreeItem& item);
  const DeviceModel parseDevice(const TreeItem& item);
  const QMap<quint16, CommandSettings> parseCommands(const TreeItem& item);

  QScopedPointer<ParserWorker> m_parseWorker;
  QMap<quint16, DeviceModel> m_DeviceModels;
  QStringList m_baudrates;
  QMultiMap<uint, QString> m_commondDevicesId;
};

#endif  // DEVICEMODELFACTORY_H
