#include "staticstyles.h"

QString StaticStyles::buttonStyle() {
  return R"(
        QPushButton {
            background-color: #3498db;
            color: white;
            border-radius: 5px;
            padding: 8px 15px;
        }
        QPushButton:hover {
            background-color: #2980b9;
        }
        QPushButton:pressed {
            background-color: #1f6391;
        }
    )";
}

// Стиль для метки
QString StaticStyles::labelStyle() {
  return R"(
        QLabel {
            color: #2c3e50;
            font-size: 14px;
            font-weight: bold;
        }
    )";
}

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

QString StaticStyles::deviceWidget() {
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
    border: 2px solid #1A1A1A;
    border-radius: 13px;
    background:  #FAFAFA;
}
QWidget#widgetBox{
background-color: #FAFAFA;
}
)";
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
