#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void updateButtonPressed();

signals:
    void startUpdate();
    void showConsoleSignal(bool state);

private:
    Ui::AboutDialog *ui;
    QString secretCode;
};

#endif // ABOUTDIALOG_H
