#ifndef UPDATEWIDGET_H
#define UPDATEWIDGET_H

#include <QMessageBox>
#include <QWidget>
#include <QtCore>

#include "UpdatesChecker.h"

class UpdateWidget : public QWidget {
  Q_OBJECT
 public:
  explicit UpdateWidget(QWidget* parent = nullptr);

 private:
  UpdatesChecker* m_updater;

 signals:
  void updateApp(QCoreApplication* app);

 private slots:
  void acceptError(const QString& errorMsg);
  void updateCheck(bool state);
};

#endif  // UPDATEWIDGET_H
