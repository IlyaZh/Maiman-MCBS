#pragma once

#include <QDebug>
#include <QLabel>
#include <QMap>
#include <QWidget>

#include "gui/guiinterface.h"

struct DeviceStatusGroup {
  std::optional<QStringList> errors;
  std::optional<QStringList> interlocks;
  std::optional<QMap<QString, bool>> devStarted;
};

namespace Ui {
class GroupWidgetStatus;
}
class GroupWidgetStatus : public QWidget, public GuiWidgetInterface {
  Q_OBJECT

 public:
  explicit GroupWidgetStatus(QWidget *parent = nullptr);
  ~GroupWidgetStatus();
  void addData(DeviceStatusGroup &status);
  void setModel(const QString &name);
  void setName(const QString &id);
  void setLink(bool status);
  void updateStyle() override;

 private:
  Ui::GroupWidgetStatus *ui;
  static const QString startedStyleOn;
  static const QString startedStyleOff;
  QMap<QString, QLabel *> m_devs;
};
