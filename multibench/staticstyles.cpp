#include "staticstyles.h"

namespace StyleStorage {
namespace MainWindow {
const QString AppWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QMainWindow *{
        background: #1D1D1D;
        color: #FFFFFF;
}
QMenu:hover {
        color: #969696;
}

QMenu::item:selected {
        background-color: #515151;
}

QMenu::item:disabled {
        color: #787878;
}
)";
  } else {
    return R"(
QMainWindow *{
        background: #FAFAFA;
        color: #000000;
}
QMenu:hover {
        color: #969696;
}

QMenu::item:selected {
        background-color: #E7E7E7;
}

QMenu::item:disabled {
        color: #FAFAFA;
}
)";
  }
}
const QString menuBarWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QMenuBar {
        background: rgb(51,51,51);
        color: #fff;
}

QMenuBar::item {
        color: #fff;
        background: rgb(51,51,51);
}

QMenuBar::item:disabled {
        color: #999;
}

QMenuBar::item:selected { /* when selected using mouse or keyboard */
      background: rgb(111,111,111);
 }

QMenuBar::item:hover { /* when selected using mouse or keyboard */
      background: rgb(111,111,111);
     color: #eee;
 }

 QMenuBar::item:pressed {
      background: rgb(31,31,31);
 }
)";
  } else {
    return R"(
QMenuBar {
        background: #FAFAFA;
        color: #000000;
}

QMenuBar::item {
        color: #000000;
        background: #FAFAFA;
}

QMenuBar::item:disabled {
        color: #969696;
}

QMenuBar::item:selected { /* when selected using mouse or keyboard */
      background: #E7E7E7;
 }

QMenuBar::item:hover { /* when selected using mouse or keyboard */
      background: #E7E7E7;
     color: #000000;
 }

 QMenuBar::item:pressed {
      background: #F3F3F3;
 }
)";
  }
}
const QString changeStyleButton() {
  if (!AppSettings::getDarkAppStyle()) {
    return R"(
    QCheckBox{
        background-color: #FFFFFF;
        border-radius: 11px;
        border: 1px solid #A7A7A7;
        width: 50px;
        height: 24px;
}
    QCheckBox::indicator {
        width: 20px;
        height: 20px;
    }

    QCheckBox::indicator::unchecked {
        background-color: #ffc803;
        border-radius: 10px;
        border: 1px solid #ffc803;
    }

    QCheckBox::indicator::checked {
        background-color: #000000;
        border-radius: 10px;
        border: 1px solid #000000;
    }

    QCheckBox::indicator::unchecked {
        /*position: absolute;*/
        left: 2px;
        content: "";
        width: 19px;
        height: 18px;
        background-color: #ffc803;
        border-radius: 10px;
        transition: all 0.3s;
    }

    QCheckBox::indicator::checked {
        /*position: absolute;*/
        left: 26px;
        content: "";
        width: 19px;
        height: 18px;
        background-color: #000000;
        border-radius: 10px;
        transition: all 0.3s;
    }
)";
  } else {
    return R"(
QCheckBox{
        background-color: #ffc803;
        border-radius: 11px;
        border: 1px solid #ffc803;
        width: 50px;
        height: 24px;
}
QCheckBox::indicator {
        width: 20px;
        height: 20px;
    }

    QCheckBox::indicator::unchecked {
        background-color: #ffc803;
        border-radius: 10px;
        border: 1px solid #ffc803;
    }

    QCheckBox::indicator::checked {
        background-color: #000000;
        border-radius: 10px;
        border: 1px solid #000000;
    }

    QCheckBox::indicator::unchecked {
        /*position: absolute;*/
        left: 2px;
        content: "";
        width: 19px;
        height: 18px;
        background-color: #ffc803;
        border-radius: 10px;
        transition: all 0.3s;
    }

    QCheckBox::indicator::checked {
        /*position: absolute;*/
        left: 26px;
        content: "";
        width: 19px;
        height: 18px;
        background-color: #000000;
        border-radius: 10px;
        transition: all 0.3s;
    }
)";
  }
}
}  // namespace MainWindow
namespace Device {
const QString widget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      QWidget#DeviceWidget {
      border: 2px solid #1A1A1A;
      border-radius: 13px;
      background:  #282828;
      }
      QWidget#widgetBox{
      background-color: #282828;
}
)";
  } else {
    return R"(
      QWidget#DeviceWidget {
      border: 2px solid #F3F3F3;
      border-radius: 13px;
      background:  #FFFFFF;
      }
      QWidget#widgetBox{
      background-color: #FFFFFF;
      }
)";
  }
}
const QString hiddenWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      QWidget {
      background-color: #282828;
}
)";
  } else {
    return R"(
      QWidget {
      background-color: #FFFFFF;
}
)";
  }
}
const QString hideButton() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      border: 2px solid #1A1A1A;
      border-radius: 5px;
      background: #1D1D1D;
      color: #FFFFFF;
      padding: 0px;
      text-align:center;
      margin-top: 5px;
      margin-left: 10px;
      margin-bottom: 10px;
)";
  } else {
    return R"(
      border: 2px solid #E6E6E6;
      border-radius: 5px;
      background: #E6E6E6;
      color: #000000;
      padding: 0px;
      text-align:center;
      margin-top: 5px;
      margin-left: 10px;
      margin-bottom: 10px;
)";
  }
}
const QString pinButton() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      border: 1px solid #1D1D1D;
      border-radius: 3px;
      background: #1D1D1D;
      color: rgb(255,255,255);
      padding: 0px;
      margin-right: 10px;
      margin-bottom: 5px;
      margin-left: 10px;
    )";
  } else {
    return R"(
      border: 1px solid #E7E7E7;
      border-radius: 3px;
      background: #E7E7E7;
      color: rgb(255,255,255);
      padding: 0px;
      margin-right: 10px;
      margin-bottom: 5px;
      margin-left: 10px;
    )";
  }
}

const QString condition() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      color: #FFC803;
      background: #282828;
      padding: 0 10px;
      border: 2px solid #1A1A1A;
      border-top-width: 0px;
      border-bottom-left-radius: 13px;
      border-bottom-right-radius: 13px;
)";
  } else {
    return R"(
      color: #FFC803;
      background: #FFFFFF;
      padding: 0 10px;
      border: 2px solid #F3F3F3;
      border-top-width: 0px;
      border-bottom-left-radius: 13px;
      border-bottom-right-radius: 13px;
)";
  }
}
const QString model() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      QLabel#modelLabel{
      color:#FFFFFF;
      background-color: #282828;
      }
)";
  } else {
    return R"(
      QLabel{
      color:#000000;
      background-color: #FFFFFF;
      }
)";
  }
}

const QString linkConnected() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    QLabel {
    background: #39CA76;
    border: 0px solid #1A1A1A;
    border-radius: 3px;
}
)";
  } else {
    return R"(
    QLabel {
    background: #3BBB5B;
    border: 1px solid rgba(59,187,91, 40%);
    border-radius: 3px;
}
)";
  }
}

const QString linkDisconnected() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    QLabel {
    background: #FF403A;
    border: 1px solid #1A1A1A;
    border-radius: 3px;
}
)";
  } else {
    return R"(
    QLabel {
    background: #FF2E27;
    border: 0px solid #1A1A1A;
    border-radius: 3px;
}
)";
  }
}
namespace InlineEdit {
const QString widgetConnected() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      QLineEdit{background-color: #282828;
      color: #39CA76;
      border-color: #6B6B6B;
      border: 1px;
      border-radius: 6px;
}
)";
  } else {
    return R"(
      QLineEdit{background-color: #FFFFFF;
      color: #3BBB5B;
      border-color: #FFFFFF;
      border: 1px;
      border-radius: 6px;
}
)";
  }
}
const QString widgetDisconnected() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      QLineEdit{background-color: #282828;
      color: #FF403A;
      border-color: #6B6B6B;
      border: 1px;
      border-radius: 6px;
}
)";
  } else {
    return R"(
      QLineEdit{background-color: #FFFFFF;
      color: #FF2E27;
      border-color: #FFFFFF;
      border: 1px;
      border-radius: 6px;
}
)";
  }
}

const QString widgetEditConnected() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      QLineEdit{background-color: #222222;
      color: #39CA76;
      border-color: #6B6B6B;
      border: 1px;
      border-radius: 6px;
}
)";
  } else {
    return R"(
      QLineEdit{background-color: #F3F3F3;
      color: #3BBB5B;
      border-color: #F3F3F3;
      border: 1px;
      border-radius: 6px;
}
)";
  }
}
const QString widgetEditDisconnected() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      QLineEdit{background-color: #222222;
      color: #FF403A;
      border-color: #6B6B6B;
      border: 1px;
      border-radius: 6px;
}
)";
  } else {
    return R"(
      QLineEdit{background-color: #F3F3F3;
      color: #FF2E27;
      border-color: #F3F3F3;
      border: 1px;
      border-radius: 6px;
}
)";
  }
}
const QString button() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      border: 0px solid #1A1A1A;
      border-radius: 3px;
      background: #1A1A1A;
      color: #8E8E8E;
      padding: 0px;
      margin-bottom: 0px;
)";
  } else {
    return R"(
      border: 1px solid #E7E7E7;
      border-radius: 3px;
      background: #E7E7E7;
      color: #8E8E8E;
      padding: 0px;
      margin-bottom: 0px;
)";
  }
}
}  // namespace InlineEdit
namespace BinaryWidget {
const QString widget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      QWidget {
      background-color: #282828;
      color: #999999;
}
)";
  } else {
    return R"(
      QWidget {
      background-color: #FFFFFF;
      color: #6B6B6B;
}
)";
  }
}
const QString checkBox() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      QCheckBox{
          border-color: #000000;
          color: #999999;
          spacing: 8;
      }
      QCheckBox::indicator {
          width: 12px;
          height: 12px;
          padding-bottom: 4;
      }
      QCheckBox::indicator:unchecked {
          image: url(:/resources/images/DarkCheckBoxOff.png);
      }
      QCheckBox::indicator:unchecked:hover {
          image: url(:/resources/images/DarkCheckBoxOff.png);
      }
      QCheckBox::indicator:unchecked:pressed {
          image: url(:/resources/images/DarkCheckBoxOff.png);
      }
      QCheckBox::indicator:checked {
          image: url(:/resources/images/DarkCheckBoxOn.png);
      }
      QCheckBox::indicator:checked:hover {
          image: url(:/resources/images/DarkCheckBoxOn.png);
      }
      QCheckBox::indicator:checked:pressed {
          image: url(:/resources/images/DarkCheckBoxOn.png);
      }
      QCheckBox::indicator:indeterminate:hover {
          image: url(:/resources/images/DarkCheckBoxOff.png);
      }
      QCheckBox::indicator:indeterminate:pressed {
          image: url(:/resources/images/DarkCheckBoxOff.png);
      }
)";
  } else {
    return R"(
      QCheckBox{
          border-color: rgb(0, 0, 0);
          color: #6B6B6B;
          spacing: 8;
      }
      QCheckBox::indicator {
          width: 12px;
          height: 12px;
          padding-bottom: 4;
      }
      QCheckBox::indicator:unchecked {
          image: url(:/resources/images/LightCheckBoxOff.png);
      }
      QCheckBox::indicator:unchecked:hover {
          image: url(:/resources/images/LightCheckBoxOff.png);
      }
      QCheckBox::indicator:unchecked:pressed {
          image: url(:/resources/images/LightCheckBoxOff.png);
      }
      QCheckBox::indicator:checked {
          image: url(:/resources/images/LightCheckBoxOn.png);
      }
      QCheckBox::indicator:checked:hover {
          image: url(:/resources/images/LightCheckBoxOn.png);
      }
      QCheckBox::indicator:checked:pressed {
          image: url(:/resources/images/LightCheckBoxOn.png);
      }
      QCheckBox::indicator:indeterminate:hover {
          image: url(:/resources/images/LightCheckBoxOff.png);
      }
      QCheckBox::indicator:indeterminate:pressed {
          image: url(:/resources/images/LightCheckBoxOff.png);
      }
)";
  }
}
}  // namespace BinaryWidget
namespace ReadParameter {
const QString widget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QWidget{
      background-color: #282828;
      color: #999999;
}
)";
  } else {
    return R"(
QWidget{
      background-color: #FFFFFF;
      color: #6B6B6B;
}
)";
  }
}
}  // namespace ReadParameter
namespace ControlWidget {
const QString widget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      background-color: #282828;
)";
  } else {
    return R"(
      background-color: #FFFFFF;
)";
  }
}
namespace InlineControl {
const QString read() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    QLineEdit{background-color: #222222;
    border-radius: 12px;
    border: 1px solid #6B6B6B;
    color: rgba(255, 255, 255, 90%);
    text-align: center;
}
)";
  } else {
    return R"(
    QLineEdit{background-color: #FFFFFF;
    border-radius: 12px;
    border: 1px solid #A7A7A7;
    color: #000000;
    text-align: center;
}
)";
  }
}
const QString edit() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    QLineEdit{background-color: #6B6B6B;
    border-radius: 12px;
    border: 1px solid #6B6B6B;
    color: rgba(255, 255, 255, 90%);
    text-align: center;
}
)";
  } else {
    return R"(
    QLineEdit{background-color: #F0F0F0;
    border-radius: 12px;
    border: 1px solid #A7A7A7;
    color: #000000;
    text-align: center;
}
)";
  }
}
const QString error() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    QLineEdit{background-color: #FF403A;
    border-radius: 12px;
    border: 1px solid #6B6B6B;
    color: #000000;
    text-align: center;
}
)";
  } else {
    return R"(
    QLineEdit{background-color: #FA3D3A;
    border-radius: 12px;
    border: 1px solid #A7A7A7;
    color: #000000;
    text-align: center;
}
)";
  }
}

const QString button() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      border: 0px solid rgba(26,26,26,10%);
      border-radius: 11px;
      background: rgba(26,26,26,10%);
      color: #8E8E8E;
      padding: 0px;
      margin-bottom: 0px;
)";
  } else {
    return R"(
      border: 0px solid rgba(26,26,26,10%);
      border-radius: 11px;
      background: rgba(240,240,240,10%);
      color: #000000;
      padding: 0px;
      margin-bottom: 0px;
)";
  }
}
}  // namespace InlineControl
const QString nameWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      color: #FFFFFF;
      background-color: #282828;
      padding: 0px;
      margin: 0px;
      alignment-baseline="middle";
)";
  } else {
    return R"(
      color: #000000;
      background-color: #FFFFFF;
      padding: 0px;
      margin: 0px;
      alignment-baseline="middle";
)";
  }
}
const QString realLabels() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      color:#FFFFFF;
)";
  } else {
    return R"(
      color:#000000;
)";
  }
}
const QString maxAndMinLabels() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      color:#6C6C6C;
)";
  } else {
    return R"(
      color:#B7B7B7;
)";
  }
}
}  // namespace ControlWidget
namespace ButtonWidget {
const QString buttonStarted() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      border: 0px solid #1A1A1A;
      border-radius: 17px;
      color: #000000;
      text-align:center;
      padding-top: 7px;
      background: #39CA76;
)";
  } else {
    return R"(
      border: 2px solid #3BBB5B;
      border-radius: 17px;
      color: #000000;
      text-align:center;
      padding-top: 7px;
      background: #B6E9C3;
)";
  }
}
const QString buttonStopped() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      border: 0px solid #1A1A1A;
      border-radius: 17px;
      color: #000000;
      text-align:center;
      padding-top: 7px;
      background: #FF403A;
)";
  } else {
    return R"(
      border: 2px solid #FA3D3A;
      border-radius: 17px;
      color: #000000;
      text-align:center;
      padding-top: 7px;
      background: #FFB6B5;
)";
  }
}
}  // namespace ButtonWidget
}  // namespace Device
namespace Group {
namespace Widget {
const QString groupWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      QWidget#GroupWidget {
          border: 0px solid rgb(26,26,26);
          border-radius: 13px;
          background: rgb(51,51,51);
      }
      QLabel {
          color: rgb(153,153,153);
      }
)";
  } else {
    return R"(
      QWidget#GroupWidget {
          border: 0px solid #FAFAFA;
          border-radius: 13px;
          background: #FAFAFA;
      }
      QLabel {
          color: #000000;
      }
)";
  }
}
const QString buttonLaunched() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      border: 0px solid #39CA76;
      border-radius: 17px;
      color: rgb(0,0,0);
      background: #39CA76;
      text-align:center;
)";
  } else {
    return R"(
      border: 2px solid #3BBB5B;
      border-radius: 17px;
      color: rgb(0,0,0);
      background: #B6E9C3;
      text-align:center;
)";
  }
}
const QString buttonStopped() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      border: 0px solid #FF403A;
      border-radius: 17px;
      color: #000000;
      background: #FF403A;
      text-align:center;
)";
  } else {
    return R"(
      border: 2px solid #FA3D3A;
      border-radius: 17px;
      color: #000000;
      background: #FFB6B5;
      text-align:center;
)";
  }
}
const QString buttonInMiddle() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      border: 0px solid #DEDDDD;
      border-radius: 17px;
      color: #000000;
      background: #DEDDDD;
      text-align:center;
)";
  } else {
    return R"(
      border: 2px solid #BBBBBB;
      border-radius: 17px;
      color: #000000;
      background: #EAEAEA;
      text-align:center;
)";
  }
}
const QString hideButton() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      border: 1px solid #1A1A1A;
      border-radius: 5px;
      background: #1D1D1D;
      color: #FFFFFF;
      text-align:center;
)";
  } else {
    return R"(
      border: 1px solid #E6E6E6;
      border-radius: 5px;
      background: #E6E6E6;
      color: #000000;
      text-align:center;
)";
  }
}
}  // namespace Widget
namespace Manager {}
namespace Status {}
}  // namespace Group
}  // namespace StyleStorage
