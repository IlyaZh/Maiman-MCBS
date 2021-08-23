#include "parameterwidget.h"
#include "ui_parameterwidget.h"
#include <QDebug>

ParameterWidget::ParameterWidget(const Control &settings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParameterWidget),
    m_settings(settings)
{
    ui->setupUi(this);

    if(m_settings.max == 0) {
        ui->maxLabel->setVisible(false);
    }

    if(m_settings.min == 0) {
        ui->minLabel->setVisible(false);
    }

    if(m_settings.real == 0) {
        ui->realLabel->setVisible(false);
    }

    if(m_settings.value == 0) {
        ui->valueLabel->setVisible(false);
    }


    ui->nameLabel->setText(m_settings.name);
}

ParameterWidget::~ParameterWidget()
{
    delete ui;
}

const Control& ParameterWidget::settings() {
    return m_settings;
}

void ParameterWidget::setValue(quint16 cmd, double value) {
    QString valueStr = QString::number(value, 'g', m_decimals.value(cmd));
    if(cmd == m_settings.max) {
        ui->maxLabel->setText(valueStr);
    } else if (cmd == m_settings.min) {
        ui->minLabel->setText(valueStr);
    } else if (cmd == m_settings.real) {
        ui->realLabel->setText(valueStr);
    } else if (cmd == m_settings.value) {
        ui->valueLabel->setText(valueStr);
    }
}

void ParameterWidget::setValue(quint16 cmd, int value) {
    if(cmd == m_settings.max) {
        ui->maxLabel->setNum(value);
    } else if (cmd == m_settings.min) {
        ui->minLabel->setNum(value);
    } else if (cmd == m_settings.real) {
        ui->realLabel->setNum(value);
    } else if (cmd == m_settings.value) {
        ui->valueLabel->setNum(value);
    }
}

void ParameterWidget::setUnit(const QString& unit) {
    m_unit = unit;
    ui->unitLabel->setText(unit);
    ui->nameLabel->setText(QString("%1, %2").arg(m_settings.name, m_unit));
}

void ParameterWidget::setDecimals(quint16 cmd, int decimals) {
    m_decimals.insert(cmd, decimals);
    if(cmd == m_settings.max) {

    } else if (cmd == m_settings.min) {

    } else if (cmd == m_settings.real) {

    } else if (cmd == m_settings.value) {

    }
}

// private slots

void ParameterWidget::spinBoxEditingFinished() {}
