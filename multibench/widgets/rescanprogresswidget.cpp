#include "rescanprogresswidget.h"
#include "ui_rescanprogresswidget.h"

#include <QtCore>

RescanProgressWidget::RescanProgressWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RescanProgressWidget)
{
    ui->setupUi(this);
}

RescanProgressWidget::~RescanProgressWidget()
{
    delete ui;
}

void RescanProgressWidget::setProgress(int current, int total) {
    QString message = QString("%1/%2").arg(current).arg(total);
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(current);
    ui->infoLabel->setText("Devices");
    ui->descriptionLabel->setText(message);

}

void RescanProgressWidget::notFound(){
    ui->descriptionLabel->setText("");
    ui->infoLabel->setText("Devices not found");
}
