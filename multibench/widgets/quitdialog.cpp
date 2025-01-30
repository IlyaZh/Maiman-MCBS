#include "quitdialog.h"

#include <QQmlApplicationEngine>
#include <QtQuickControls2>

#include "ui_quitdialog.h"

QuitDialog::QuitDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::QuitDialog) {
  ui->setupUi(this);
  this->setModal(true);
  setWindowTitle("Confirm");
  setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint |
                 Qt::WindowCloseButtonHint);
  ui->buttonBox->layout()->setSpacing(6);
  ui->label->setContentsMargins(0, 0, 0, 0);
  QFont font18("Poppins", 18);
  font18.setLetterSpacing(QFont::PercentageSpacing, 105);
  font18.setPixelSize(18);
  ui->label->setFont(font18);
  QuitDialog::updateStyle();
  //  adjustSize();
}

QuitDialog::~QuitDialog() { delete ui; }

QuitDialog* QuitDialog::createDialog(const QString& title, QString message,
                                     QDialogButtonBox::StandardButtons buttons,
                                     int font, QWidget* parent) {
  QuitDialog* dialog = new QuitDialog(parent);
  dialog->updateStyle();
  dialog->setWindowTitle(title);
  dialog->ui->label->setText(message);
  dialog->ui->label->setFont(QFont("Poppins", font));
  dialog->adjustSize();
  dialog->ui->buttonBox->setStandardButtons(buttons);
  dialog->show();

  return dialog;
}

void QuitDialog::updateStyle() {
  this->setStyleSheet(StyleStorage::MainWindow::Quit::quitWidget());
  ui->buttonBox->setStyleSheet(StyleStorage::MainWindow::Quit::buttons());
  ui->label->setStyleSheet(StyleStorage::MainWindow::Quit::label());
  this->update();
}
