#include "quitdialog.h"

#include <QQmlApplicationEngine>
#include <QtQuickControls2>

#include "ui_quitdialog.h"

QuitDialog::QuitDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::QuitDialog) {
  ui->setupUi(this);
  setWindowTitle("Confirm");
  setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
  ui->buttonBox->layout()->setSpacing(9);
  ui->label->setContentsMargins(0, 0, 0, 5);
  adjustSize();
}

QuitDialog::~QuitDialog() { delete ui; }

QuitDialog* QuitDialog::createDialog(const QString& title, QString message,
                                     QDialogButtonBox::StandardButtons buttons,
                                     int font, QWidget* parent) {
  QuitDialog* dialog = new QuitDialog(parent);
  dialog->setWindowTitle(title);
  dialog->ui->label->setText(message);
  dialog->ui->label->setFont(QFont("Share Tech Mono", font));
  dialog->adjustSize();
  dialog->ui->buttonBox->setStandardButtons(buttons);
  dialog->show();

  return dialog;
}
