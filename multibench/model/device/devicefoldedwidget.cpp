#include "devicefoldedwidget.h"

#include "ui_devicefoldedwidget.h"

DeviceFoldedWidget::DeviceFoldedWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::DeviceFoldedWidget) {
  ui->setupUi(this);
  connect(ui->showButton, &QPushButton::clicked, this,
          &DeviceFoldedWidget::showWidget);
}

DeviceFoldedWidget::~DeviceFoldedWidget() { delete ui; }
