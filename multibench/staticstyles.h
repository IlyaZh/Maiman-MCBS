#pragma once

#include <QObject>

#include "appsettings.h"

class StaticStyles : public QObject {
  Q_OBJECT
 public:
  static QString buttonStyle();
  static QString labelStyle();
  static QString mainWindowStyle();
  static QString deviceWidget();
  static QString aboutDialog();
  static QString binaryWidget();
  static QString buttonWidget();
  static QString calibrationWidget();
  static QString calibrationDialog();
  static QString connectionWidget();
  static QString controlWidget();
  static QString groupManager();
  static QString groupStatusDialog();
  static QString groupWidget();
  static QString groupStatusWidget();
  static QString inLineEdit();
  static QString quitDialog();
  static QString readParameterWidget();
  static QString rescanProgressWidget();

 private:
  StaticStyles() = delete;
};
