#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog {
  Q_OBJECT

 public:
  InputDialog(QString title, QString name, QWidget *parent = nullptr);
  ~InputDialog();
  void setLimits(int min, int value, int max);
 signals:
  void changesCompleted(int value);

 private:
  Ui::InputDialog *ui;
  int m_step = 1;
};

#endif  // INPUTDIALOG_H
