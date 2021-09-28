#include "devicewidget.h"
#include "ui_devicewidget.h"
#include <QDebug>

#include "model/device/parameterwidget.h"
#include "device/devicecommand.h"
#include <QStyleOption>
#include <QPainter>

DeviceWidget::DeviceWidget(const DeviceWidgetDesc& description, const QVector<DevCommand*>& commands, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceWidget),
    m_description(description),
    m_commands(commands)
{
    ui->setupUi(this);

    for(auto control : description.controls) {
        DevCommand* cmd = nullptr;
        if((cmd = m_commands.value(control.max, nullptr)) != nullptr) {

        }

        auto widget = new ParameterWidget(control, this);
        m_paramWidgets.append(widget);
        ui->paramLayout->addWidget(widget, 0, m_paramWidgets.size());
        // TODO: переработай это всё, я ведь новую форму сделал
        // TODO: добавь создание кнопки для каждого такого виджета кроме тока
        connect(widget, &ParameterWidget::valueChanged, this, &DeviceWidget::parameterChanged);
    }

    //    connect(m_device, SIGNAL(dataToView(quint16, double)), this, SLOT(dataChanged(quint16, double)));
    //    connect(m_device, SIGNAL(dataToView(quint16, int)), this, SLOT(dataChanged(quint16, int)));
}

DeviceWidget::~DeviceWidget()
{
    delete ui;
}

void DeviceWidget::setAddress(int addr) {
    ui->idLabel->setText(QString("ID:%1").arg(addr));
}

// public slots


void DeviceWidget::setValue(quint16 reg, double value) {
    qDebug() << "DeviceWidget" << reg << value;
    for (auto paramWid : m_paramWidgets) {
        paramWid->setValue(reg, value);
    }
}

void DeviceWidget::parameterChanged(quint16 reg, double value) {
    qDebug() << "parameterChanged" << reg << value;
    emit dataChanged(reg, value);
}

// private methods

void DeviceWidget::paintEvent(QPaintEvent*) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//void DeviceWidget::setValue(quint16 reg, int value) {}
