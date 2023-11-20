#ifndef PLUSMINUSWIDGET_H
#define PLUSMINUSWIDGET_H

#include <QDialog>
#include <QDoubleValidator>
#include <QLabel>
#include <QLayout>
#include <QWidget>

#include "commandconverter.h"
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
  explicit PlusMinusWidget(const CalibrationKoef &calibration,
                           QSharedPointer<CommandConverter> converter,
                           QWidget *parent = nullptr);
  explicit PlusMinusWidget(const Limit &limit,
                           QSharedPointer<DevCommand> command,
                           QSharedPointer<DevCommand> maxCommand,
                           QSharedPointer<DevCommand> minCommand,
                           QWidget *parent = nullptr);
  explicit PlusMinusWidget(const Limit &limit,
                           QSharedPointer<CommandConverter> converter,
                           QSharedPointer<CommandConverter> converterMax,
                           QSharedPointer<CommandConverter> converterMin,
                           QWidget *parent = nullptr);
  ~PlusMinusWidget() override;
  void sendValue();
  bool state();
  double value();
  void setMin(double min);
  void setMax(double max);
  void setData(quint16 code, quint16 data);
  QVector<quint16> Subscribe();

 private slots:
  void increment();
  void decrement();

  void rejectedEdit();
  void inputCompleted();
  void textChanged();
 signals:
  void lineEditTextChanged();
  void setDataFromWidget(quint16 code, quint16 value);

 private:
  void keyPressEvent(QKeyEvent *event) override;
  void validateValue();
  QSharedPointer<DevCommand> m_command;
  QSharedPointer<CommandConverter> m_converter;
  QSharedPointer<CommandConverter> m_converterMax;
  QSharedPointer<CommandConverter> m_converterMin;
  Ui::CalibrationAndLimitsWidget *ui;
  QDoubleValidator *m_validator;
  QVector<quint16> m_codes;

  double minValue = 0;
  double maxValue = 0;
  double delta = 0;
  bool m_state = true;
  bool m_textChanged = false;

  static const QString styleSheetOK;
  static const QString styleSheetERROR;
};

#endif  // PLUSMINUSWIDGET_H
