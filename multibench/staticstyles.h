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
const QString linkConnected();
const QString linkDisconnected();
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
namespace ControlWidget {
const QString widget();
namespace InlineControl {
const QString read();
const QString edit();
const QString error();
const QString button();
}  // namespace InlineControl
const QString nameWidget();
const QString realLabels();
const QString maxAndMinLabels();
}  // namespace ControlWidget
}  // namespace Device
namespace About {
const QString widget();
}

}  // namespace StyleStorage
