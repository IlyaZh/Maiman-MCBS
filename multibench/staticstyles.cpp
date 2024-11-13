#include "staticstyles.h"

QString StaticStyles::mainWindowStyle() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
* {
        background:#1D1D1D;
        color: rgb(255, 255, 255);
}

QMainWindow *{
        background: #1D1D1D;
        color: rgb(255, 255, 255);
}

QPushButton {
        color: rgb(255, 255, 255);
        background-color: #1D1D1D;
        border: 1px solid #999999;
        border-radius: 4px;
        padding: 4px;
}
/*
QMenu:checked {
        color: rgb(150, 150, 150);
}
*/
QMenu:hover {
        color: rgb(150, 150, 150);
}

QMenu::item:selected {
        background-color: rgb(81, 81, 81);
}

QMenu::item:disabled {
        color: rgb(120, 120, 120);
}
    )";
  } else {
    return R"(
* {
        background:#FAFAFA;
        color: rgb(255, 255, 255);
}

QMainWindow *{
        background: #FAFAFA;
        color: rgb(255, 255, 255);
}

QPushButton {
        color: rgb(255, 255, 255);
        background-color: #1D1D1D;
        border: 1px solid #999999;
        border-radius: 4px;
        padding: 4px;
}
/*
QMenu:checked {
        color: rgb(150, 150, 150);
}
*/
QMenu:hover {
        color: rgb(150, 150, 150);
}

QMenu::item:selected {
        background-color: rgb(81, 81, 81);
}

QMenu::item:disabled {
        color: rgb(120, 120, 120);
}
    )";
  }
}

QString StaticStyles::binaryWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QCheckBox{
    border-color: rgb(0, 0, 0);
    color: rgb(153, 153, 153);
    spacing: 8;
}
QCheckBox::indicator {
    width: 12px;
    height: 12px;
    padding-bottom: 4;
}
QCheckBox::indicator:unchecked {
    image: url(:/resources/images/checkBoxOff.png);
}
QCheckBox::indicator:unchecked:hover {
    image: url(:/resources/images/checkBoxOff.png);
}
QCheckBox::indicator:unchecked:pressed {
    image: url(:/resources/images/checkBoxOff.png);
}
QCheckBox::indicator:checked {
    image: url(:/resources/images/checkBoxOn.png);
}
QCheckBox::indicator:checked:hover {
    image: url(:/resources/images/checkBoxOn.png);
}
QCheckBox::indicator:checked:pressed {
    image: url(:/resources/images/checkBoxOn.png);
}
QCheckBox::indicator:indeterminate:hover {
    image: url(:/resources/images/checkBoxOff.png);
}
QCheckBox::indicator:indeterminate:pressed {
    image: url(:/resources/images/checkBoxOff.png);
}
)";
  } else {
    return R"(
QCheckBox{
    border-color: rgb(0, 0, 0);
    color: rgb(153, 153, 153);
    spacing: 8;
}
QCheckBox::indicator {
    width: 12px;
    height: 12px;
    padding-bottom: 4;
}
QCheckBox::indicator:unchecked {
    image: url(:/resources/images/checkBoxOff.png);
}
QCheckBox::indicator:unchecked:hover {
    image: url(:/resources/images/checkBoxOff.png);
}
QCheckBox::indicator:unchecked:pressed {
    image: url(:/resources/images/checkBoxOff.png);
}
QCheckBox::indicator:checked {
    image: url(:/resources/images/checkBoxOn.png);
}
QCheckBox::indicator:checked:hover {
    image: url(:/resources/images/checkBoxOn.png);
}
QCheckBox::indicator:checked:pressed {
    image: url(:/resources/images/checkBoxOn.png);
}
QCheckBox::indicator:indeterminate:hover {
    image: url(:/resources/images/checkBoxOff.png);
}
QCheckBox::indicator:indeterminate:pressed {
    image: url(:/resources/images/checkBoxOff.png);
}
)";
  }
}

QString StaticStyles::buttonWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    QWidget#DeviceWidget {
    border: 2px solid #1A1A1A;
    border-radius: 13px;
    background:  #282828;
})";
  } else {
    return R"(
    QWidget#DeviceWidget {
    border: 2px solid #1A1A1A;
    border-radius: 13px;
    background:  #FAFAFA;
})";
  }
}

QString StaticStyles::aboutDialog() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    background: #333333;
    color: #fff;
)";
  } else {
    return R"(
    background: #333333;
    color: #fff;
)";
  }
}

QString StaticStyles::calibrationWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QDialog#CalibrationAndLimitsWidget{
background: rgb(51,51,51);
color: rgb(255, 255, 255);
}
QLineEdit#value {
        font: 16pt "Share Tech Mono";
        color: rgb(16, 33, 40);
        background: rgb(255, 255, 255);
        border-radius: 5px;
        padding: 5px 0;
}

QLineEdit::disabled#value {
        background: rgb(255, 255, 255);
        color: rgb(76, 93, 100);
        border-radius: 5px;
        padding: 5px 0;
}
QPushButton#upButton {
        background: rgb(16, 33, 40);
        color: rgb(230, 230, 230);
        border-radius: 5px;
        width: 20px;
        height: 30px;

        font: 16pt "Share Tech Mono";
}

QPushButton::disabled#upButton {
        background: rgb(76, 93, 100);
        color: rgb(180, 180, 180);
}
QPushButton#downButton {
        background: rgb(16, 33, 40);
        color: rgb(230, 230, 230);
        border-radius: 5px;
        width: 20px;
        height: 30px;

        font: 16pt "Share Tech Mono";
}

QPushButton::disabled#downButton {
        background: rgb(76, 93, 100);
        color: rgb(180, 180, 180);
}

)";
  } else {
    return R"(
QDialog#CalibrationAndLimitsWidget{
background: rgb(51,51,51);
color: rgb(255, 255, 255);
}
QLineEdit#value {
        font: 16pt "Share Tech Mono";
        color: rgb(16, 33, 40);
        background: rgb(255, 255, 255);
        border-radius: 5px;
        padding: 5px 0;
}

QLineEdit::disabled#value {
        background: rgb(255, 255, 255);
        color: rgb(76, 93, 100);
        border-radius: 5px;
        padding: 5px 0;
}
QPushButton#upButton {
        background: rgb(16, 33, 40);
        color: rgb(230, 230, 230);
        border-radius: 5px;
        width: 20px;
        height: 30px;

        font: 16pt "Share Tech Mono";
}

QPushButton::disabled#upButton {
        background: rgb(76, 93, 100);
        color: rgb(180, 180, 180);
}
QPushButton#downButton {
        background: rgb(16, 33, 40);
        color: rgb(230, 230, 230);
        border-radius: 5px;
        width: 20px;
        height: 30px;

        font: 16pt "Share Tech Mono";
}

QPushButton::disabled#downButton {
        background: rgb(76, 93, 100);
        color: rgb(180, 180, 180);
}
)";
  }
}

QString StaticStyles::calibrationDialog() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QDialog{
        background: rgb(51,51,51);
}
QLabe {
        color: #fff;
}

QPushButton {
        color: rgb(255, 255, 255);
        background-color: rgb(51, 51, 51);
        border: 1px solid #999999;
        border-radius: 4px;
        padding: 4px;
        width: 50px
}
)";
  } else {
    return R"(
QDialog{
        background: rgb(51,51,51);
}
QLabe {
        color: #fff;
}

QPushButton {
        color: rgb(255, 255, 255);
        background-color: rgb(51, 51, 51);
        border: 1px solid #999999;
        border-radius: 4px;
        padding: 4px;
        width: 50px
}
)";
  }
}

QString StaticStyles::connectionWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QWidget#ConnectionWidget{
background-color: rgb(51, 51, 51);
color: rgb(255, 255, 255);
}

QComboBox {
background: rgb(51,51,51);
color: #ffffff;
border: 1px solid #FFFFFF;
border-radius: 4px;
padding-left: 10px;
padding-right: 10px;
}

QPushButton {
background: rgb(51,51,51);
color: #ffffff;
border: 1px solid #FFFFFF;
border-radius: 4px;
padding-left: 10px;
padding-right: 10px;
}


)";
  } else {
    return R"(
QWidget#ConnectionWidget{
background-color: rgb(51, 51, 51);
color: rgb(255, 255, 255);
}

QComboBox {
background: rgb(51,51,51);
color: #ffffff;
border: 1px solid #FFFFFF;
border-radius: 4px;
padding-left: 10px;
padding-right: 10px;
}

QPushButton {
background: rgb(51,51,51);
color: #ffffff;
border: 1px solid #FFFFFF;
border-radius: 4px;
padding-left: 10px;
padding-right: 10px;
}
)";
  }
}

QString StaticStyles::controlWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(

QWidget#ControlWidget {
    background-color: #282828;
}
QLineEdit{
background-color: #222222;
border-radius: 12px;
border: 1px solid #6B6B6B;
color: #FFFFFF;
text-align: center;
}

QLabel#WidetName{
color: #FFFFFF;
background-color: #282828;
padding: 0px;
margin: 0px;
alignment-baseline="middle";
}

QLabel#Max{
color:#6C6C6C
}
QLabel#MaxUnits{
color:#6C6C6C
}
QLabel#MaxValue{
color:#6C6C6C
}

QLabel#Min{
color:#6C6C6C
}
QLabel#MinUnits{
color:#6C6C6C
}
QLabel#MinValue{
color:#6C6C6C
}

QLabel#Real{
color:#6C6C6C
}
QLabel#RealUnits{
color:#6C6C6C
}
QLabel#RealValue{
color:#6C6C6C
}
)";
  } else {
    return R"(
QWidget#ControlWidget {
    background-color: #FAFAFA;
}
QLineEdit{
background-color: #FAFAFA;
border-radius: 12px;
border: 1px solid #6B6B6B;
color: #FFFFFF;
text-align: center;
}

QLabel#WidetName{
color: #000000;
background-color: #282828;
padding: 0px;
margin: 0px;
alignment-baseline="middle";
}

QLabel#Max{
color:#B7B7B7
}
QLabel#MaxUnits{
color:#B7B7B7
}
QLabel#MaxValue{
color:#B7B7B7
}

QLabel#Min{
color:#B7B7B7
}
QLabel#MinUnits{
color:#B7B7B7
}
QLabel#MinValue{
color:#B7B7B7
}

QLabel#Real{
color:#000000
}
QLabel#RealUnits{
color:#000000
}
QLabel#RealValue{
color:#000000
}
)";
  }
}

QString StaticStyles::groupManager() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
* {
        background: rgb(51,51,51);
        color: rgb(255, 255, 255);
}

QPushButton{
border: 2px solid rgb(26,26,26);
border-radius: 6px;
color: rgb(0,0,0);
background: rgb(180,180,180);
}
)";
  } else {
    return R"(
* {
        background: rgb(51,51,51);
        color: rgb(255, 255, 255);
}

QPushButton{
border: 2px solid rgb(26,26,26);
border-radius: 6px;
color: rgb(0,0,0);
background: rgb(180,180,180);
}
)";
  }
}

QString StaticStyles::groupStatusDialog() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QDialog{
        background: rgb(51,51,51);
}
QLabel {
        color: #fff;
}

QPushButton {
        color: rgb(255, 255, 255);
        background-color: rgb(51, 51, 51);
        border: 1px solid #999999;
        border-radius: 4px;
        padding: 4px;
        width: 50px
}
)";
  } else {
    return R"(
QDialog{
        background: rgb(51,51,51);
}
QLabel {
        color: #fff;
}

QPushButton {
        color: rgb(255, 255, 255);
        background-color: rgb(51, 51, 51);
        border: 1px solid #999999;
        border-radius: 4px;
        padding: 4px;
        width: 50px
}
)";
  }
}

QString StaticStyles::groupWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QWidget#GroupWidget {
border: 2px solid rgb(26,26,26);
border-radius: 5px;
background: rgb(51,51,51);
}
QLabel {
    color: rgb(153,153,153);
}
)";
  } else {
    return R"(
QWidget#GroupWidget {
border: 2px solid rgb(26,26,26);
border-radius: 5px;
background: rgb(51,51,51);
}
QLabel {
    color: rgb(153,153,153);
}
)";
  }
}

QString StaticStyles::groupStatusWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QWidget#GroupWidgetStatus {
    border: 1px solid black;
    border-radius: 5px;
    background:  rgb(51,51,51);
}
QLabel {
    color: rgb(255,255,255);
}
)";
  } else {
    return R"(
QWidget#GroupWidgetStatus {
    border: 1px solid black;
    border-radius: 5px;
    background:  rgb(51,51,51);
}
QLabel {
    color: rgb(255,255,255);
}
)";
  }
}

QString StaticStyles::inLineEdit() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QLineEdit{background-color: #282828;\
    color: rgb(255,255,255);\
    border-color: #6B6B6B;\
    border: 1px;\
}
)";
  } else {
    return R"(
QLineEdit{background-color: #FAFAFA;\
    color: #000000;\
    border-color: #6B6B6B;\
    border: 1px;\
}
)";
  }
}

QString StaticStyles::quitDialog() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QDialog{
        background: rgb(51,51,51);
}
QLabel {
        color: #fff;
}

QPushButton {
        color: rgb(255, 255, 255);
        background-color: rgb(51, 51, 51);
        border: 1px solid #999999;
        border-radius: 4px;
        padding: 4px;
        width: 50px
}
)";
  } else {
    return R"(
QDialog{
        background: rgb(51,51,51);
}
QLabel {
        color: #fff;
}

QPushButton {
        color: rgb(255, 255, 255);
        background-color: rgb(51, 51, 51);
        border: 1px solid #999999;
        border-radius: 4px;
        padding: 4px;
        width: 50px
}
)";
  }
}

QString StaticStyles::readParameterWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QWidget {
    background-color:#282828;
    /*color: rgb(153,153,153);*/
}
QLabel {
    color: rgb(153,153,153);
}
)";
  } else {
    return R"(
QWidget {
    background-color:#282828;
    /*color: rgb(153,153,153);*/
}
QLabel {
    color: rgb(153,153,153);
}
)";
  }
}

QString StaticStyles::rescanProgressWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(

QProgressBar {
background-color: rgb(81, 81, 81);
    border: 2px;
    border-radius: 3px;
    text-align: center;
}
QProgressBar::chunk {
    background-color: rgb(255, 255, 255);
    border: 2px;
    border-radius: 3px;
}

QLabel#infoLabel{
color: rgb(153, 153, 153);
}

QLabel#descriptionLabel{
color: rgb(153, 153, 153);
}
)";
  } else {
    return R"(
QProgressBar {
background-color: rgb(81, 81, 81);
    border: 2px;
    border-radius: 3px;
    text-align: center;
}
QProgressBar::chunk {
    background-color: rgb(255, 255, 255);
    border: 2px;
    border-radius: 3px;
}

QLabel#infoLabel{
color: rgb(153, 153, 153);
}

QLabel#descriptionLabel{
color: rgb(153, 153, 153);
}
)";
  }
}

namespace StyleStorage {
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
}  // namespace StyleStorage
