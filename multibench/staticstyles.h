#pragma once

#include <QObject>

#include "appsettings.h"

class StaticStyles : public QObject {
  Q_OBJECT
 public:
  static QString mainWindowStyle();
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

namespace StyleStorage {
namespace Device {
const QString widget();
const QString hiddenWidget();
const QString hideButton();
const QString pinButton();
const QString condition();
const QString model();
namespace InlineEdit {
const QString widgetConnected();
const QString widgetDisconnected();
const QString widgetEditConnected();
const QString widgetEditDisconnected();
const QString button();
}  // namespace InlineEdit
namespace BinaryWidget {
const QString widget();
const QString checkBox();
}  // namespace BinaryWidget
namespace ReadParameter {
const QString widget();
}  // namespace ReadParameter
}  // namespace Device
namespace About {
const QString widget();
}

}  // namespace StyleStorage
