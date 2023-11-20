#ifndef CalibrateDialog_H
#define CalibrateDialog_H

#include <QDialog>

#include "device/commandsettings.h"
#include "model/device/devicewidget.h"

class PlusMinusWidget;
struct CalibrationKoef;

namespace Ui {
class CalibrationDialog;
}  // namespace Ui

class CalibrationDialog : public QDialog {
  Q_OBJECT

 public:
  explicit CalibrationDialog(
      const DeviceWidgetDesc& deviceDesc,
      const QMap<quint16, QSharedPointer<DevCommand>>& commands,
      QWidget* parent = nullptr);
  explicit CalibrationDialog(
      const DeviceWidgetDesc& deviceDesc,
      const QMap<quint16, QSharedPointer<CommandConverter>>& converters,
      QWidget* parent = nullptr);
  ~CalibrationDialog() override;
  void setValues();
  void updateValue(const model::Event& event);

 signals:
  void acceptDataFromWidget(quint16 code, quint16 value);

 private:
  QVector<PlusMinusWidget*> m_calibrationWidgets;
  QVector<PlusMinusWidget*> m_limitsWidgets;
  Ui::CalibrationDialog* ui;

  static const QString styleButtonOn;
  static const QString styleButtonOff;
 private slots:
  void saveResult();
  void widgetsAreValid();

 signals:
  void sendData(QString);
};

#endif  // CalibrateDialog_H
