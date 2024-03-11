#ifndef GROUPSTATUSDIALOG_H
#define GROUPSTATUSDIALOG_H

#include <QDialog>
#include <QLayout>

#include "widgets/groupwidgetstatus.h"

namespace Ui {
class GroupStatusDialog;
}

class GroupStatusDialog : public QDialog {
  Q_OBJECT

 public:
  explicit GroupStatusDialog(QWidget* parent = nullptr);
  ~GroupStatusDialog();
  void addDevice(quint8 addr, const QString& name, const QString& model);
 public slots:
  void setStatus(QMap<quint8, DeviceStatusGroup>& status);
  void deviceLinkChanged(int addr, bool status);

 private:
  Ui::GroupStatusDialog* ui;
  QMap<quint8, QSharedPointer<GroupWidgetStatus>> m_devices;
  QVBoxLayout* m_devicesLayout;
};

#endif  // GROUPSTATUSDIALOG_H
