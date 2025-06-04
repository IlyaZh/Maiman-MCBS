#ifndef GROUPWIDGETFOLDED_H
#define GROUPWIDGETFOLDED_H

#include <QWidget>

#include "group/GroupInterface.h"
#include "gui/DeviceSmallWidgets.h"
#include "gui/guiinterface.h"
#include "model/device/deviceholder.h"
#include "widgets/groupstatusdialog.h"
#include "widgets/inlineedit.h"
#include "widgets/warningwidget.h"

namespace Ui {
class GroupWidgetFolded;
}

class GroupWidgetFolded : public QWidget, public GuiWidgetInterface {
  Q_OBJECT

 public:
  explicit GroupWidgetFolded(QWidget *parent = nullptr);
  ~GroupWidgetFolded();
  void updateStyle() override;
  void setName(QString name);
  void linkStatusChanged(bool status);
  void addDevicesData(QMap<quint8, QSharedPointer<DeviceStatusGroup>> &status);
 signals:
  void startAll(bool checked);
  void stopAll(bool checked);
  bool showWidget(bool flag);

 private:
  Ui::GroupWidgetFolded *ui;
  QPushButton *m_showButton;
  WarningWidget *m_warning;
};

#endif  // GROUPWIDGETFOLDED_H
