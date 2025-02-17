#pragma once

#include <QObject>

#include "appsettings.h"

namespace StyleStorage {
namespace MainWindow {
const QString AppScrollBars();
const QString AppWidget();
const QString menuBarWidget();
const QString scrollWidget();
const QString changeStyleSlider();
const QString inputDialog();
namespace Quit {
const QString quitWidget();
const QString label();
const QString buttons();
}  // namespace Quit
namespace Connection {
const QString widget();
const QString comboBoxes();
const QString labels();
const QString buttons();
}  // namespace Connection
namespace Rescan {
const QString widget();
const QString name();
const QString labels();
const QString progressBar();
}  // namespace Rescan
namespace About {
const QString widget();
const QString credits();
const QString logo();
const QString buttons();
}  // namespace About
}  // namespace MainWindow
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
namespace ButtonWidget {
const QString buttonStarted();
const QString buttonStopped();
}  // namespace ButtonWidget
}  // namespace Device
namespace Calibration {
namespace Dialog {
const QString generalButtons();
const QString buttonEnabled();
const QString buttonDisabled();
}  // namespace Dialog
namespace widget {
const QString lineEditOk();
const QString lineEditError();
}  // namespace widget
}  // namespace Calibration
namespace About {
const QString widget();
}
namespace Group {
namespace Widget {
const QString groupWidget();
const QString buttonLaunched();
const QString buttonStopped();
const QString buttonInMiddle();
const QString hideButton();
namespace InlineEdit {
const QString widgetConnected();
const QString widgetDisconnected();
const QString widgetEditConnected();
const QString widgetEditDisconnected();
const QString button();
}  // namespace InlineEdit
}  // namespace Widget
namespace Manager {
const QString devsField();
const QString groupsField();
const QString checkBoxes();
}  // namespace Manager
namespace Status {
const QString widget();
const QString labels();
const QString unactiveLabels();
const QString activeLabels();
const QString nameLabelOn();
const QString nameLabelOff();
const QString linkOn();
const QString linkOff();
const QString warningGray();
const QString warningYellow();
const QString warningRed();
}  // namespace Status
}  // namespace Group
}  // namespace StyleStorage
