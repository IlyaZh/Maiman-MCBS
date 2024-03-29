#ifndef PLUSMINUSWIDGET_H
#define PLUSMINUSWIDGET_H

#include <QDialog>
#include <QDoubleValidator>
#include <QLabel>
#include <QLayout>
#include <QWidget>

#include "device/commandsettings.h"

struct CalibrationKoef;
struct Limit;
namespace Ui {
class CalibrationAndLimitsWidget;
}

class PlusMinusWidget : public QDialog {
  Q_OBJECT

 public:
  explicit PlusMinusWidget(const CalibrationKoef &calibration,
                           QSharedPointer<DevCommand> command,
                           QWidget *parent = nullptr);
  explicit PlusMinusWidget(const Limit &limit,
                           QSharedPointer<DevCommand> command,
                           QSharedPointer<DevCommand> maxCommand,
                           QSharedPointer<DevCommand> minCommand,
                           QWidget *parent = nullptr);
  ~PlusMinusWidget() override;
  void sendValue();
  bool state();
  double value();
  void setMin(double min);
  void setMax(double max);

 private slots:
  void increment();
  void decrement();

  void rejectedEdit();
  void inputCompleted();
  void textChanged();
 signals:
  void lineEditTextChanged();

 private:
  void keyPressEvent(QKeyEvent *event) override;
  void validateValue();
  QSharedPointer<DevCommand> m_command;
  Ui::CalibrationAndLimitsWidget *ui;
  QDoubleValidator *m_validator;

  double minValue = 0;
  double maxValue = 0;
  double delta = 0;
  bool m_state = true;

  static const QString styleSheetOK;
  static const QString styleSheetERROR;
};

#endif  // PLUSMINUSWIDGET_H
