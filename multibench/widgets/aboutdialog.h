#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class AboutDialog;
}  // namespace Ui

class AboutDialog : public QDialog {
  Q_OBJECT

 public:
  explicit AboutDialog(QWidget *parent = 0);
  ~AboutDialog() override;
  void keyReleaseEvent(QKeyEvent *event) override;

 private slots:
  void updateButtonPressed();

 signals:
  void startUpdate();
  void showConsoleSignal(bool state);

 private:
  Ui::AboutDialog *ui;
  QString secretCode{};
};

#endif  // ABOUTDIALOG_H
