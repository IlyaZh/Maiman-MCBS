#include "quitdialog.h"
#include "ui_quitdialog.h"
#include <QQmlApplicationEngine>
#include <QtQuickControls2>


QuitDialog::QuitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuitDialog)
{
    ui->setupUi(this);
    setWindowTitle("Confirm");
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ui->buttonBox->layout()->setSpacing(9);
    ui->label->setContentsMargins(0,0,0,5);
    adjustSize();
    //ui->buttonBox->layout()->setContentsMargins(0,0,0,5);
}

QuitDialog::~QuitDialog()
{
    delete ui;
}

QuitDialog* QuitDialog::createDialog(const QString& title, QString message, QDialogButtonBox::StandardButtons buttons, int font, QWidget *parent){
    QuitDialog* dialog = new QuitDialog(parent);
    dialog->setWindowTitle(title);
    dialog->ui->label->setText(message);
    dialog->ui->label->setFont(QFont("Share Tech Mono", font));
    dialog->adjustSize();
    dialog->ui->buttonBox->setStandardButtons(buttons);
    dialog->show();
    /*
    int side = 300;
    QRegion maskedRegion(dialog->width() / 2 - side / 2, dialog->height() / 2 - side / 2, side, side, QRegion::Ellipse);
    dialog->setMask(maskedRegion);
    dialog->setWindowFlag(Qt::FramelessWindowHint);

    QQuickView *view = new QQuickView();
    QWidget *container = QWidget::createWindowContainer(view);
    container->setMinimumSize(200, 200);
    container->setMaximumSize(200, 200);
    view->setSource(QUrl("main.qml"));
    view->show();
*/
    return dialog;
}
