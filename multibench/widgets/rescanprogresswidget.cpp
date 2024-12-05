#include "rescanprogresswidget.h"

#include <QtCore>

#include "ui_rescanprogresswidget.h"

RescanProgressWidget::RescanProgressWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::RescanProgressWidget) {
  ui->setupUi(this);
  RescanProgressWidget::updateStyle();
}

RescanProgressWidget::~RescanProgressWidget() { delete ui; }

void RescanProgressWidget::setProgress(int current, int total, int success) {
  QString message = QString("%1/%2").arg(success).arg(total);
  ui->progressBar->setMaximum(total);
  ui->progressBar->setValue(current);
  ui->infoLabel->setText("Devices");
  ui->descriptionLabel->setText(message);
}

void RescanProgressWidget::notFound() {
  ui->descriptionLabel->setText("");
  ui->infoLabel->setText("Devices not found");
}

void RescanProgressWidget::updateStyle() {
  ui->label->setStyleSheet(StyleStorage::MainWindow::Rescan::name());
  ui->descriptionLabel->setStyleSheet(
      StyleStorage::MainWindow::Rescan::labels());
  ui->infoLabel->setStyleSheet(StyleStorage::MainWindow::Rescan::labels());
  ui->progressBar->setStyleSheet(
      StyleStorage::MainWindow::Rescan::progressBar());
  this->setStyleSheet(StyleStorage::MainWindow::Rescan::widget());
  this->update();
}
