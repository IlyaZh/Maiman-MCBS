#include "staticstyles.h"

namespace StyleStorage {
namespace MainWindow {
const QString AppScrollBars() {
  return R"(
    QScrollArea {
        background: transparent;
        border: none;
    }
    QScrollBar:vertical, QScrollBar:horizontal {
        background: #1D1D1D;
        width: 12px; /* Ширина вертикального */
        height: 12px; /* Высота горизонтального */
        margin: 0px;
    }
    QScrollBar::handle:vertical, QScrollBar::handle:horizontal {
        background: #2E2E2E;
        border-radius: 3px;
        min-height: 20px; /* Минимальный размер для вертикального */
        min-width: 20px;  /* Минимальный размер для горизонтального */
    }
    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical,
    QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
        background: none;
        border: none;
        width: 0px;
        height: 0px;
    }
    QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical,
    QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
        background: none;
    }
)";
}
const QString AppWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QMainWindow *{
        background: #1D1D1D;
        color: #FFFFFF;
}
QMainWindow{
        border: 0px solid #1A1A1A;
        border-bottom-left-radius: 13px;
        border-bottom-right-radius: 13px;
}
QMenu:hover {
        color: #969696;
}

QMenu::item:selected {
      background: #1D1D1D;
      color: #FFC803;
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
        background: #282828;
        color: #fff;
}

QMenuBar::item {
        color: #fff;
        background: #282828;
}

QMenuBar::item:disabled {
        color: #9C9C9C;
}

QMenuBar::item:selected { /* when selected using mouse or keyboard */
      background: #1D1D1D;
      color: #FFC803;
 }

QMenuBar::item:hover { /* when selected using mouse or keyboard */
      background: #1D1D1D;
      color: #FFC803;
 }

 QMenuBar::item:pressed {
      background: #1D1D1D;
      color: #FFC803;
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

const QString scrollWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
          background: #1D1D1D;
          color: #FFFFFF;
 )";
  } else {
    return R"(
          background: #FAFAFA;
          color: #000000;
  )";
  }
}

const QString changeStyleSlider() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
        QSlider{
        background-color: #ffc803;
        border-radius: 12px;
        border: 1px solid #ffc803;
        width: 48px;
        height: 22px;
        }

        QSlider::groove:horizontal {
        border: 1px solid #ffc803;
        height: 18px;
        border-radius: 9px;
        background: #ffc803;
        margin-left: 2px;
        margin-right: 2px;
        }

        QSlider::handle:horizontal {
        background:  #000000;
        border: 0px solid #000000;
        width: 20px;
        height: 19px;
        border-radius: 9px;
        }
  )";
  } else {
    return R"(
        QSlider{
        background-color: #FAFAFA;
        border-radius: 12px;
        border: 1px solid #A7A7A7;
        width: 48px;
        height: 22px;
        }

        QSlider::groove:horizontal {
        border: 1px solid #FAFAFA;
        height: 18px;
        border-radius: 9px;
        background: #FAFAFA;
        margin-left: 2px;
        margin-right: 2px;
        }

        QSlider::handle:horizontal {
        background:  #ffc803;
        border: 0px solid #ffc803;
        width: 20px;
        height: 19px;
        border-radius: 9px;
        }
  )";
  }
}
const QString inputDialog() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QDialog#CustomDialog{
background-color: #1D1D1D;
border: 0px solid red;
border-radius: 13px;
}
   )";
  } else {
    return R"(
QDialog{
background-color:#1D1D1D;
border: 0px solid #1D1D1D;
border-bottom-left-radius: 13px;
border-bottom-right-radius: 13px;
}
    )";
  }
}

namespace Quit {
const QString quitWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    background: #1D1D1D;
  )";
  } else {
    return R"(
    background: #FFFFFF;
  )";
  }
}
const QString label() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    background: #1D1D1D;
    color: #FFFFFF;
  )";
  } else {
    return R"(
    background: #FFFFFF;
    color: #000000;
  )";
  }
}
const QString buttons() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QPushButton{
    color: #FFFFFF;
    background-color: #1D1D1D;
    border: 1px solid #6b6b6b;
border-radius: 15px;
width: 91px;
height: 31px;
margin: 5px;
}
QPushButton:hover{
    color: #1D1D1D;
    background-color: #FFC803;
}
  )";
  } else {
    return R"(
    color: #000000;
    background-color: #FFFFFF;
    border: 1px solid #A7A7A7;
    border-radius: 4px;
    padding: 4px;
    width: 50px

  )";
  }
}
}  // namespace Quit
namespace Connection {
const QString widget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
        background-color: #1D1D1D;
        color: #FFFFFF;
  )";
  } else {
    return R"(
        background-color: #FAFAFA;
        color: #000000;
  )";
  }
}
const QString comboBoxes() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QComboBox{
background-color: rgb(0, 0, 0);
color: #ffffff;
border: 1px solid #393838;
border-radius: 5px;
padding-left: 10px;
padding-right: 10px;
}
QComboBox::drop-down{
background-color: rgb(0, 0, 0);
color: #ffffff;
border: 0px solid #393838;
border-radius: 5px;
}
QComboBox::down-arrow {
            width: 5px;
            height: 3px;
            image: url(:/resources/images/white-arrow-down.png);
background: transparent;
}
QComboBox QAbstractItemView {
    border: 1px solid #393838;
    border-radius: 5px;
    selection-background-color: #6B6B6B;
    selection-color: black;
    background: black;
    color: white;
}
QComboBox::item:selected {
    background: #6B6B6B;;
    color: white;
}
  )";
  } else {
    return R"(
        background: #FAFAFA;
        color: #000000;
        border: 1px solid #1D1D1D;
        border-radius: 4px;
        padding-left: 10px;
        padding-right: 10px;
  )";
  }
}
const QString labels() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
        color: #FFFFFF;
  )";
  } else {
    return R"(
        color: #000000;
  )";
  }
}
const QString buttons() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QPushButton{
        background: #1D1D1D;
        color: #ffffff;
        border: 1px solid #6B6B6B;
        border-radius: 15px;
        padding-left: 10px;
        padding-right: 10px;
}
QPushButton:hover{
    color: #000000;
    background-color: #FFC803;
    border: 1px solid #FFC803;
}
  )";
  } else {
    return R"(
        background: #FAFAFA;
        color: #000000;
        border: 1px solid #FFFFFF;
        border-radius: 4px;
        padding-left: 10px;
        padding-right: 10px;
  )";
  }
}
}  // namespace Connection
namespace Rescan {
const QString widget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
          color: #1D1D1D;
    )";
  } else {
    return R"(
          color: #FAFAFA;
    )";
  }
}
const QString name() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
        color: #FFFFFF;
    )";
  } else {
    return R"(
        color: #000000;
    )";
  }
}
const QString labels() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
        color: #999999;
    )";
  } else {
    return R"(
        color: #999999;
    )";
  }
}
const QString progressBar() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
        QProgressBar {
        background-color: #4A4A4A;
            border: 0px solid #999999;
            border-radius: 4px;
            text-align: center;
        }
        QProgressBar::chunk {
            background-color: #FFC803;
            border: 0px solid #999999;
            border-radius: 4px;
        }
    )";
  } else {
    return R"(
        QProgressBar {
        background-color: #FAFAFA;
            border: 2px;
            border-radius: 3px;
            text-align: center;
        }
        QProgressBar::chunk {
            background-color: #000000;
            border: 2px;
            border-radius: 3px;
        }
    )";
  }
}
}  // namespace Rescan
namespace About {
const QString widget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
        background: #1D1D1D;
        color: #FFFFFF;
        border-bottom-left-radius: 13px;
        border-bottom-right-radius: 13px;
      )";
  } else {
    return R"(
        background: #FAFAFA;
        color: #000000;
      )";
  }
}
const QString credits() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
        color: #9A9A9A;
        font: 10pt "Poppins";
      )";
  } else {
    return R"(
        color: #000000;
      )";
  }
}
const QString logo() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
        QWidget {
            image: url(:/resources/images/maiman-logo-minimal.png);
        }
      )";
  } else {
    return R"(
        QWidget {
            image: url(:/resources/images/maiman-logo-minimal.png);
        }
      )";
  }
}
const QString buttons() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
        QPushButton {
            color: rgb(255, 255, 255);
            background-color:#1D1D1D;
            border: 1px solid #6B6B6B;
            border-radius: 15px;
        }
      )";
  } else {
    return R"(
        QPushButton {
            color: #000000;
            background-color: #E7E7E7;
            border: 1px solid #E7E7E7;
            border-radius: 4px;
            padding: 4px;
        }
      )";
  }
}
}  // namespace About
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
    padding-left: 10px;
    padding-top: 2px;
      border: 2px solid #1A1A1A;
      border-top-width: 0px;
      border-bottom-left-radius: 13px;
      border-bottom-right-radius: 13px;
)";
  } else {
    return R"(
      color: #FFC803;
      background: #FFFFFF;
    padding-left: 10px;
    padding-top: 2px;
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
      padding-top: 2px;
      background: #39CA76;
)";
  } else {
    return R"(
      border: 2px solid #3BBB5B;
      border-radius: 17px;
      color: #000000;
      text-align:center;
      padding-top: 2px;
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
      padding-top: 2px;
      background: #FF403A;
)";
  } else {
    return R"(
      border: 2px solid #FA3D3A;
      border-radius: 17px;
      color: #000000;
      text-align:center;
      padding-top: 2px;
      background: #FFB6B5;
)";
  }
}
}  // namespace ButtonWidget
}  // namespace Device
namespace Calibration {
namespace Dialog {
const QString generalButtons() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QPushButton {
    color: rgb(255, 255, 255);
    background-color: #1D1D1D;
    border: 1px solid #6B6B6B;
    border-radius: 15px;
    padding: 4px;
    width: 91px;
    height: 31px;
}

QPushButton::hover{
color: #000000;
    background-color: #FFC803;
    border: 1px solid #FFC803;
}
)";
  } else {
    return R"(

)";
  }
}
const QString buttonEnabled() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QPushButton {
    color: rgb(255, 255, 255);
    background-color: #1D1D1D;
    border: 1px solid #6B6B6B;
    border-radius: 15px;
    padding: 4px;
    width: 91px;
    height: 31px;
}

QPushButton::hover{
    color: #000000;
    background-color: #FFC803;
    border: 1px solid #FFC803;
}
  )";
  } else {
    return R"(

  )";
  }
}
const QString buttonDisabled() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QPushButton {
    color: rgb(255, 255, 255);
    background-color: #6F6F6F;
    border: 1px solid #6F6F6F;
    border-radius: 15px;
    padding: 4px;
    width: 91px;
    height: 31px;
}

QPushButton::hover{

}
  )";
  } else {
    return R"(

  )";
  }
}
}  // namespace Dialog
namespace widget {
const QString lineEditOk() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    QLineEdit {
        color: #FFFFFF;
        background: #000000;
        border: 1px solid #414141;
        border-radius: 12px;
        padding-top:2px;
    }
    QLineEdit::disabled {
        background: #000000;
        color: #FFFFFF;
        border: 1px solid #414141;
        border-radius: 12px;
        padding-top:2px;
    }
    QLineEdit:focus {
        color: #000000;
        background: #FFFFFF;
        border: 1px solid #414141;
        border-radius: 12px;
        padding-top:2px;
    }
)";
  } else {
    return R"(

)";
  }
}
const QString lineEditError() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      QLineEdit {
          color: #FF403A;
          background: #000000;
          border: 1px solid #414141;
          border-radius: 12px;
padding-top:2px;
      }
      QLineEdit::disabled {
          background: #FF403A;
          color: #FFFFFF;
          border: 1px solid #414141;
          border-radius: 12px;
padding-top:2px;
      }
      QLineEdit:focus {
          color: #FF403A;
          background: #FFFFFF;
          border: 1px solid #414141;
          border-radius: 12px;
padding-top:2px;
      }
  )";
  } else {
    return R"(

  )";
  }
}
}  // namespace widget
}  // namespace Calibration
namespace Group {
namespace Widget {
const QString groupWidget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
        QWidget#GroupWidget {
        border: 2px solid #1A1A1A;
        border-radius: 13px;
        background: #232323;
        }
        QWidget#devicesTable{
        background:#232323;
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
      border: 1px solid #565656;
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
namespace InlineEdit {
const QString widgetConnected() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
      QLineEdit{background-color: #232323;
      color: #39CA76;
      border-color: #232323;
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
      QLineEdit{background-color: #232323;
      color: #FF403A;
      border-color: #232323;
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
      QLineEdit{background-color: #6B6B6B;
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
      QLineEdit{background-color: #6B6B6B;
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
      border: 0px solid #232323;
      border-radius: 3px;
      background: #232323;
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
}  // namespace Widget
namespace Manager {}
namespace Status {
const QString widget() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QWidget#GroupWidgetStatus{
    border: 0px solid red;
    border-radius: 13px;
    background:  #000000;
}
)";
  } else {
    return R"(
QWidget#GroupWidgetStatus{
    border: 5px solid red;
    border-radius: 5px;
    background:  #000000;
}

)";
  }
}
const QString labels() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    color: #FFFFFF;
    font-size: 17px;
    padding-top: 3px;
    margin-left: 18px;
  )";
  } else {
    return R"(
    color: #FFFFFF;
    font: 16px;
  )";
  }
}

const QString unactiveLabels() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    color: #808080;
    font-size: 17px;
    padding-top: 3px;
    margin-left: 18px;
  )";
  } else {
    return R"(
    color: #FFFFFF;
    font: 16px;
  )";
  }
}

const QString activeLabels() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    color: #FFFFFF;
    font-size: 17px;
    padding-top: 3px;
  )";
  } else {
    return R"(
    color: #FFFFFF;
    font: 16px;
  )";
  }
}

const QString nameLabelOn() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QLabel{
    color: #39CA76;
    font-size: 17px;
    padding-top: 3px;
}
    )";
  } else {
    return R"(
      color: #FFFFFF;
      font: 16px;
    )";
  }
}
const QString nameLabelOff() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QLabel{
    color: #FF2E27;
    font-size: 17px;
    padding-top: 3px;
}
    )";
  } else {
    return R"(
      color: #FFFFFF;
      font: 16px;
    )";
  }
}
const QString linkOn() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QLabel{
        background: #39CA76;
        border: 1px solid #39CA76;
        border-radius: 3px;
}
    )";
  } else {
    return R"(
      color: #FFFFFF;
      font: 16px;
    )";
  }
}
const QString linkOff() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
QLabel{
        background: #FF2E27;
        border: 1px solid #FF2E27;
        border-radius: 3px;
}
    )";
  } else {
    return R"(
      color: #FFFFFF;
      font: 16px;
    )";
  }
}

const QString warningGray() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    color: #808080;
    font-size: 17px;
    padding-top: 3px;
    margin-left: 18px;
  )";
  } else {
    return R"(
    color: #FFFFFF;
    font: 16px;
  )";
  }
}

const QString warningYellow() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    color: #FFC803;
    font-size: 17px;
    padding-top: 3px;
    margin-left: 18px;
  )";
  } else {
    return R"(
    color: #FFFFFF;
    font: 16px;
  )";
  }
}

const QString warningRed() {
  if (AppSettings::getDarkAppStyle()) {
    return R"(
    color: #FF403A;
    font-size: 17px;
    padding-top: 3px;
    margin-left: 18px;
  )";
  } else {
    return R"(
    color: #FFFFFF;
    font: 16px;
  )";
  }
}
}  // namespace Status
}  // namespace Group
}  // namespace StyleStorage
