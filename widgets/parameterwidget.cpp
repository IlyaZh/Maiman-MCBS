#include "parameterwidget.h"
#include "ui_parameterwidget.h"
#include <QDebug>
#include "devicewidget.h"
#include "absoluteToleranceCompare.h"

ParameterWidget::ParameterWidget(const Control &settings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParameterWidget),
    m_settings(settings)
{
    ui->setupUi(this);

    ui->nameLabel->setText(m_settings.name);

    connect(ui->setButton, &QPushButton::clicked, this, &ParameterWidget::setButtonPressed);

}

ParameterWidget::~ParameterWidget()
{
    delete ui;
}

const Control& ParameterWidget::settings() {
    return m_settings;
}

void ParameterWidget::setValue(quint16 cmd, double value) {
    QString valueStr = QString::number(value, 'f', m_decimals.value(cmd));
    if(cmd == m_settings.max) {
        ui->maxLabel->setText(valueStr);
        m_values.insert(cmd, value);
    } else if (cmd == m_settings.min) {
        ui->minLabel->setText(valueStr);
        m_values.insert(cmd, value);
    } else if (cmd == m_settings.real) {
        ui->realLabel->setText(valueStr);
        m_values.insert(cmd, value);
    } else if (cmd == m_settings.value) {
        ui->valueLabel->setText(valueStr);
        m_values.insert(cmd, value);
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


void ParameterWidget::setButtonPressed()
{
    bool ok;
    double value;

    if(m_decimals.value(m_settings.value) != 0)
    {
        value = QInputDialog::getDouble(this, tr("ParameterWidget"),
                                           tr("Value"), m_values.value(m_settings.value),
                                           m_values.value(m_settings.min),
                                           m_values.value(m_settings.max), 2, &ok,
                                           Qt::WindowFlags(), 1);

    }
    else
    {
        int intValue = static_cast<int>(qRound(m_values.value(m_settings.value)));
        int intMax = static_cast<int>(qRound(m_values.value(m_settings.max)));
        int intMin = static_cast<int>(qRound(m_values.value(m_settings.min)));
        value = QInputDialog::getInt(this, tr("ParameterWidget"),
                                           tr("Value"), intValue,
                                           intMin, intMax, 2, &ok,
                                           Qt::WindowFlags());

    }
    if (ok and isEqual(value, m_values.value(m_settings.value)))
    {

        emit valueChanged(m_decimals.value(m_settings.value),value);
        m_values.remove(m_settings.value);
        m_values.insert(m_settings.value, value);
    }
}

// TestWidget
TestWidget::TestWidget(QObject *parent) : QObject(parent)
{
    Control* m_settings = new Control();

    m_settings->value =    0x0300;
    m_settings->min =      0x0301;
    m_settings->max =      0x0302;
    m_settings->real =     0x0307;
    settings.reset(m_settings);
    widget.reset(new ParameterWidget(*settings.get()));
}
TestWidget::~TestWidget()
{

}

void TestWidget::testValuesInt()
{
    double testValue = 3.56;
    int testDecimals = 0;
    widget->setDecimals(settings->value, testDecimals);
    widget->setDecimals(settings->min, testDecimals);
    widget->setDecimals(settings->max, testDecimals);
    widget->setDecimals(settings->real, testDecimals);
    widget->setValue(settings->value, testValue);
    widget->setValue(settings->min, testValue);
    widget->setValue(settings->max, testValue);
    widget->setValue(settings->real, testValue);
    QString valueStr = QString::number(testValue, 'f', testDecimals);
    QString minStr = QString::number(testValue, 'f', testDecimals);
    QString maxStr = QString::number(testValue, 'f', testDecimals);
    QString realStr = QString::number(testValue, 'f', testDecimals);
    QVERIFY(valueStr == widget->ui->valueLabel->text());
    QVERIFY(minStr == widget->ui->minLabel->text());
    QVERIFY(maxStr == widget->ui->maxLabel->text());
    QVERIFY(realStr == widget->ui->realLabel->text());

}

void TestWidget::testValuesDouble()
{
    double testValue = 1.4689765;
    int testDecimals = 5;
    widget->setDecimals(settings->value, testDecimals);
    widget->setDecimals(settings->min, testDecimals);
    widget->setDecimals(settings->max, testDecimals);
    widget->setDecimals(settings->real, testDecimals);
    widget->setValue(settings->value, testValue);
    widget->setValue(settings->min, testValue);
    widget->setValue(settings->max, testValue);
    widget->setValue(settings->real, testValue);
    QString valueStr = QString::number(widget->m_values.value(settings->value)
                                       , 'f', widget->m_decimals.value(settings->value));
    QString minStr = QString::number(widget->m_values.value(settings->min)
                                       , 'f', widget->m_decimals.value(settings->min));
    QString maxStr = QString::number(widget->m_values.value(settings->max)
                                       , 'f', widget->m_decimals.value(settings->max));
    QString realStr = QString::number(widget->m_values.value(settings->real)
                                       , 'f', widget->m_decimals.value(settings->real));
    QVERIFY(valueStr == widget->ui->valueLabel->text());
    QVERIFY(minStr == widget->ui->minLabel->text());
    QVERIFY(maxStr == widget->ui->maxLabel->text());
    QVERIFY(realStr == widget->ui->realLabel->text());
}


