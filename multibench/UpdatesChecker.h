#pragma once

#include <QObject>
#include <QtCore>

class UpdatesChecker : public QObject {
  Q_OBJECT
  qint64 m_size{0};
  QString m_version{""};
  QPointer<QProcess> m_maintanceProcess;
  static const QString m_updaterFile;

 public:
  explicit UpdatesChecker(QObject* parent = nullptr);
  QString version() const;
  qint64 size() const;
  QString sizeFormated() const;
 public slots:
  void checkForUpdates();
  void startUpdate(QCoreApplication* app);
 private slots:
  void analyzeOutput();
 signals:
  void updatesAvailable(bool);
  void errorOccured(QString);

 signals:
};
