#include "binarywidget.h"
#include "ui_binarywidget.h"

BinaryWidget::BinaryWidget(const Checkbox &settings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BinaryWidget),
    m_settings(settings)
{
    ui->setupUi(this);
    ui->statusCheckbox->setText(m_settings.name);
    connect(ui->statusCheckbox, &QCheckBox::clicked, this, &BinaryWidget::checkBoxClicked);
}

BinaryWidget::~BinaryWidget()
{
    delete ui;
}

void BinaryWidget::checkBoxClicked(){
    emit valueChanged(m_settings.code,m_settings.mask,ui->statusCheckbox->checkState());
}
