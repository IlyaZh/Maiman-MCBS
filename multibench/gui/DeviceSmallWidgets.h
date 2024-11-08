#pragma once
#include <QPushButton>

#include "gui/guiinterface.h"

class DeviceHideButton : public QPushButton, public GuiWidgetInterface {
 public:
  explicit DeviceHideButton(QWidget* parent = nullptr) : QPushButton(parent) {
    setText(" " + tr("Hide controls"));
    setCheckable(true);
    setChecked(false);
    setIconSize(QSize(9, 9));
    m_darkButton.addFile(
        QString::fromUtf8(":/resources/images/DarkIconControlMask-Hide.png"),
        QSize(16, 16), QIcon::Normal, QIcon::Off);
    m_darkButton.addFile(
        QString::fromUtf8(":/resources/images/DarkIconControlMask-Show.png"),
        QSize(16, 16), QIcon::Normal, QIcon::On);
    m_lightButton.addFile(
        QString::fromUtf8(":/resources/images/LightIconControlMask-Hide.png"),
        QSize(16, 16), QIcon::Normal, QIcon::Off);
    m_lightButton.addFile(
        QString::fromUtf8(":/resources/images/LightIconControlMask-Show.png"),
        QSize(16, 16), QIcon::Normal, QIcon::On);
    setObjectName(QString::fromUtf8("hideControlButton"));
    setMinimumSize(QSize(106, 35));
    setMaximumSize(QSize(106, 35));
    QFont font1;
    font1.setFamily(QString::fromUtf8("Poppins"));
    font1.setPixelSize(10);
    font1.setLetterSpacing(QFont::PercentageSpacing, 105);
    setFont(font1);
    updateStyle();
  }
  void updateStyle() override {
    setIconSize(QSize(9, 9));
    setIcon((AppSettings::getDarkAppStyle()) ? m_darkButton : m_lightButton);
    this->setStyleSheet(StyleStorage::Device::hideButton());
    this->update();
  }

 private:
  QIcon m_darkButton;
  QIcon m_lightButton;
};

class DevicePinButton : public QPushButton, public GuiWidgetInterface {
 public:
  explicit DevicePinButton(QWidget* parent = nullptr) : QPushButton(parent) {
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy1);
    setMinimumSize(QSize(16, 16));
    setMaximumSize(QSize(16, 16));
    setMaximumHeight(20);
    m_darkButton.addFile(
        QString::fromUtf8(":/resources/images/DarkPinSymbol-disactivated.png"),
        QSize(16, 16), QIcon::Normal, QIcon::Off);
    m_darkButton.addFile(
        QString::fromUtf8(":/resources/images/DarkPinSymbol-activated.png"),
        QSize(16, 16), QIcon::Normal, QIcon::On);
    m_lightButton.addFile(
        QString::fromUtf8(":/resources/images/LightPinSymbol-disactivated.png"),
        QSize(16, 16), QIcon::Normal, QIcon::Off);
    m_lightButton.addFile(
        QString::fromUtf8(":/resources/images/LightPinSymbol-activated.png"),
        QSize(16, 16), QIcon::Normal, QIcon::On);
    setChecked(false);
    setCheckable(true);
    updateStyle();
  }
  void updateStyle() override {
    setIcon((AppSettings::getDarkAppStyle()) ? m_darkButton : m_lightButton);
    setIconSize(QSize(16, 16));
    this->setStyleSheet(StyleStorage::Device::pinButton());
    this->update();
  }

 private:
  QIcon m_darkButton;
  QIcon m_lightButton;
};
