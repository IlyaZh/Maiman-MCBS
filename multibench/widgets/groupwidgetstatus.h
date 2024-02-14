#pragma once

#include <QDebug>
#include <QLabel>
#include <QMap>
#include <QWidget>

struct DeviceStatusGroup {
  std::optional<QStringList> errors;
  std::optional<QMap<QString, bool>> devStarted;
};

namespace Ui {
class GroupWidgetStatus;
}
class GroupWidgetStatus : public QWidget {
  Q_OBJECT

 public:
  explicit GroupWidgetStatus(QWidget *parent = nullptr);
  ~GroupWidgetStatus();
  void addData(DeviceStatusGroup &status);
  void setModel(const QString &name);
  void setName(const QString &id);
  void setLink(bool status);

 private:
  Ui::GroupWidgetStatus *ui;
  static const QString startedStyleOn;
  static const QString startedStyleOff;
  QMap<QString, QLabel *> m_devs;
};
