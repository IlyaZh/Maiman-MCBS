#ifndef WARNINGWIDGET_H
#define WARNINGWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include "gui/guiinterface.h"
struct DeviceStatusGroup {
  std::optional<QStringList> errors;
  std::optional<bool> isError;
  std::optional<QStringList> interlocks;
  std::optional<bool> isInterlock;
  std::optional<bool> isStarted;
};

class CustomTooltip : public QLabel {
 public:
  explicit CustomTooltip(QWidget *parent = nullptr) : QLabel(parent) {
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    //    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet(
        "color: white; background-color: #1D1D1D; border: 1px solid "
        "#848484;border-radius: 0px; "
        "padding: 5px;");
    setAlignment(Qt::AlignCenter);
    hide();
  }

  void showTooltip(const QString &text, const QPoint &pos) {
    setText(text);
    adjustSize();
    move(pos);
    show();
  }

  void hideTooltip() { hide(); }
};

class WarningWidget : public QWidget, public GuiWidgetInterface {
  Q_OBJECT
 public:
  explicit WarningWidget(QWidget *parent = nullptr);
  explicit WarningWidget(bool inv, QWidget *parent = nullptr);
  void addData(DeviceStatusGroup &status);
  void addDevicesData(QMap<quint8, QSharedPointer<DeviceStatusGroup>> &status);
  void updateStyle() override;

 protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

 private:
  QHBoxLayout *m_layout;
  QIcon m_iconPlug;
  QIcon m_iconWarning;
  QIcon m_iconError;
  QPushButton *m_iconHolder;
  QLabel *m_warning;
  QString m_toolTip;
  CustomTooltip *m_customToolTip;
  bool m_isInv = false;
};

#endif  // WARNINGWIDGET_H
