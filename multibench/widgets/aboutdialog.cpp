#include "aboutdialog.h"

#include "constants.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AboutDialog) {
  ui->setupUi(this);
  setWindowIcon(QIcon(":/images/logo-minimal.png"));
  // Сокрытие\показ кнопки "обновления ПО" и вывод информации о текущей версии
  // программы
  ui->updateButton->hide();
  connect(ui->updateButton, SIGNAL(clicked(bool)), this,
          SLOT(updateButtonPressed()));
  ui->softwareInfoLabel->setText(QCoreApplication::applicationVersion());

  // Вызов информации о версии Qt
  connect(ui->aboutQtButton, SIGNAL(clicked(bool)), QApplication::instance(),
          SLOT(aboutQt()));
}

AboutDialog::~AboutDialog() { delete ui; }

void AboutDialog::keyReleaseEvent(QKeyEvent *event) {
  int keyPressed = event->key();
  // Заменяем запятую на точку
  keyPressed = (QChar(keyPressed) == ',') ? '.' : keyPressed;

  if (Const::kSecretCodeCombination.at(secretCode.size()) ==
      QChar(keyPressed)) {
    secretCode.append(keyPressed);
    if (secretCode.size() == Const::kSecretCodeCombination.size()) {
      if (secretCode == Const::kSecretCodeCombination) {
        emit showConsoleSignal(true);
        secretCode.clear();
      } else {
        secretCode.clear();
      }
    }
  } else {
    secretCode.clear();
  }
}

void AboutDialog::updateButtonPressed() { emit startUpdate(); }
