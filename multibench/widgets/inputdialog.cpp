#include "inputdialog.h"

#include <QDebug>

#include "ui_inputdialog.h"

InputDialog::InputDialog(QString title, QString name, QWidget *parent)
    : QDialog(parent), ui(new Ui::InputDialog) {
  ui->setupUi(this);
  this->setModal(true);
  this->setObjectName("CustomDialog");
  this->setAttribute(Qt::WA_StyledBackground, true);
  this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
  QDialog::setWindowTitle(title);
  ui->label->setText(name);
  ui->spinBox->setSingleStep(m_step);
  connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
          &InputDialog::accept);
  connect(ui->buttonBox, &QDialogButtonBox::rejected, this,
          &InputDialog::reject);
  connect(this, &InputDialog::finished, this, [this](int result) {
    switch (result) {
      case QDialog::Accepted: {
        emit changesCompleted(ui->spinBox->value());
        deleteLater();
      } break;
      case QDialog::Rejected: {
        deleteLater();
      } break;
    }
  });
  adjustSize();
}

void InputDialog::setLimits(int min, int value, int max) {
  ui->spinBox->setMinimum(min);
  ui->spinBox->setMaximum(max);
  ui->spinBox->setValue(value);
}

InputDialog::~InputDialog() { delete ui; }
