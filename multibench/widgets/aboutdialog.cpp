#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "version.h"
const QString SECRET_CODE_COMBINATION = "3.1415926";

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/logo-minimal.png"));
    // Сокрытие\показ кнопки "обновления ПО" и вывод информации о текущей версии программы
    ui->updateButton->hide();
    connect(ui->updateButton, SIGNAL(clicked(bool)), this, SLOT(updateButtonPressed()));

//    QString str = "Version: " + QString::number(MAJOR_VERSION) + "." + QString::number(MINOR_VERSION) + "." + QString::number(PATCH_VERSION);
//    if (!updateUrl.isEmpty()) {
//        str.append("<br><b>Update is available!</b>");
//        ui->updateButton->show();
//    }
    ui->softwareInfoLabel->setText(Version);

    // Вызов информации о версии Qt
    connect(ui->aboutQtButton, SIGNAL(clicked(bool)), QApplication::instance(), SLOT(aboutQt()));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::keyReleaseEvent(QKeyEvent *event) {
    int keyPressed = event->key();
    // Заменяем запятую на точку
    keyPressed = (QChar(keyPressed) == ',') ? '.' : keyPressed;

    if(SECRET_CODE_COMBINATION.at(secretCode.size()) == QChar(keyPressed)) {
        secretCode.append(keyPressed);
        if(secretCode.size() == SECRET_CODE_COMBINATION.size()) {
            if (secretCode == SECRET_CODE_COMBINATION) {
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

void AboutDialog::updateButtonPressed() {
    emit startUpdate();
}
