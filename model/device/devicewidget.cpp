#include "devicewidget.h"
#include "ui_devicewidget.h"
#include <QDebug>

//#include "model/device/parameterwidget.h"
#include "device/devicecommand.h"
#include <QStyleOption>
#include <QPainter>
#include "widgets/binarywidget.h"
#include "widgets/controlwidget.h"
#include "widgets/readparameterwidget.h"
#include "model/device/HiddenWidget.h"

DeviceWidget::DeviceWidget(const DeviceWidgetDesc& description, const QMap<quint16, QSharedPointer<DevCommand>>& commands, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceWidget),
    m_description(description),
    m_commands(commands),
    m_widgetLayout(new QGridLayout())
{
    ui->setupUi(this);

    m_widgetLayout->setMargin(6);
    m_widgetLayout->setSpacing(9);

    // Инициализация виджетов
    QVector<ReadParameterWidget*> readOnlyWidgets;
    for(const auto& control : description.controls) {
        auto valueCmd = m_commands.value(control.value, nullptr);
        auto maxCmd = m_commands.value(control.max, nullptr);
        auto minCmd = m_commands.value(control.min, nullptr);
        auto realCmd = m_commands.value(control.real, nullptr);

        if(realCmd != nullptr and valueCmd == nullptr) {
            // Обработка неизменяемых параметров
            readOnlyWidgets.append(new ReadParameterWidget(control.name, realCmd));
        } else {
            // Обработка изменяемых параметров
            auto hiddenWidget = new HiddenWidget(this);
            auto widget = new ControlWidget(control.name, valueCmd, maxCmd, minCmd, realCmd, hiddenWidget);
            m_widgetLayout->addWidget(hiddenWidget, 1, m_widgets.size());
            hiddenWidget->addWidget(widget);
            if(control.name == "current")
                hiddenWidget->setPinned(true);
            m_widgets.append(hiddenWidget);
        }
    }
    // Закидываем неизменяемые параметры в виджет
    if(readOnlyWidgets.count() > 0) {
        auto hiddenWidget = new HiddenWidget(this);
        for(auto item : readOnlyWidgets) {
            hiddenWidget->addWidget(item);
        }
        readOnlyWidgets.clear();
        m_widgetLayout->addWidget(hiddenWidget, 1, m_widgets.size());
        m_widgets.append(hiddenWidget);
    }

    // Инициализация checkbox'ов
    QPointer<HiddenWidget> hiddenWidget;
    for(const auto &item : m_description.checkboxes) {
        if(!hiddenWidget)
            hiddenWidget = new HiddenWidget(this);
        auto cmd = m_commands.value(item.code, nullptr);
        if(cmd) {
            hiddenWidget->addWidget(new BinaryWidget(item, cmd, hiddenWidget));
        }

    }
    if(hiddenWidget) {
        m_widgetLayout->addWidget(hiddenWidget.data(), 1, m_widgets.size());
        m_widgets.append(hiddenWidget);
    }

    // Инициализация кнопок pin
    for(int i = 1; i < m_widgets.count(); ++i) {
        auto pinButton = new QPushButton();
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pinButton->sizePolicy().hasHeightForWidth());
        pinButton->setSizePolicy(sizePolicy1);
        pinButton->setMinimumSize(QSize(16, 16));
        pinButton->setMaximumSize(QSize(16, 16));
        pinButton->setStyleSheet(QString::fromUtf8("border: 2px solid rgb(26,26,26);\n"
"border-radius: 3px;\n"
"background: rgb(17,17,17);\n"
"color: rgb(255,255,255);\n"
"padding: 0px;\n"
"margin: 0px;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/images/pinSymbol-disactivated.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/resources/images/pinSymbol-activated.png"), QSize(), QIcon::Normal, QIcon::On);
        pinButton->setIcon(icon);
        pinButton->setIconSize(QSize(10, 10));
        pinButton->setChecked(false);
        pinButton->setCheckable(true);
        m_pinButtons.append(pinButton);
        m_widgetLayout->addWidget(pinButton, 0, i, Qt::AlignRight);
        connect(pinButton, &QPushButton::clicked, this, [i, this](bool checked){
            pinButtonClicked(i, checked);
        });
    }

    // Инциализация кнопок Laser и TEC
    ui->laserButton->hide();
    ui->tecButton->hide();
    for(const auto &button : m_description.buttons) {
        if(button.name == "laser") {
            ui->laserButton->setVisible(true);
            ui->laserButton->setChecked(false);
            connect(ui->laserButton, &QPushButton::clicked, this, &DeviceWidget::laserButtonClicked);

            if(m_commands.contains(button.code)) {
                auto cmd = m_commands.value(button.code);
                connect(cmd.get(), &DevCommand::updatedValue, this, [this, cmd](){
                    setLaserButton(cmd->valueInt());
                });
            }
        } else if (button.name == "tec") {
            ui->tecButton->setVisible(true);
            connect(ui->tecButton, &QPushButton::clicked, this, &DeviceWidget::tecButtonClicked);
            ui->tecButton->setChecked(false);

            if(m_commands.contains(button.code)) {
                auto cmd = m_commands.value(button.code);
                connect(cmd.get(), &DevCommand::updatedValue, this, [this, cmd](){
                    setTecButton(cmd->valueInt());
                });
            }
        }
    }

    // Инициализация кнопки (Hide controls)
    auto m_hideControlsButton = new QPushButton("    "+tr("Hide controls"));
    m_hideControlsButton->setStyleSheet("border: 2px solid rgb(26,26,26);\nborder-radius: 3px;\nbackground: rgb(51,51,51);\ncolor: rgb(255,255,255);\npadding: 0px;\nmargin: 0px;");
    m_hideControlsButton->setCheckable(true);
    m_hideControlsButton->setChecked(false);
    m_hideControlsButton->setIconSize(QSize(10, 10));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/resources/images/hidecontrols-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon1.addFile(QString::fromUtf8(":/resources/images/showcontrols-icon.png"), QSize(), QIcon::Normal, QIcon::On);
    m_hideControlsButton->setIcon(icon1);
    m_hideControlsButton->setObjectName(QString::fromUtf8("hideControlButton"));
    m_hideControlsButton->setMinimumSize(QSize(135, 20));
    m_hideControlsButton->setMaximumSize(QSize(135, 20));
    QFont font1;
    font1.setFamily(QString::fromUtf8("Share Tech Mono"));
    m_hideControlsButton->setFont(font1);

    connect(m_hideControlsButton, &QPushButton::clicked, this, &DeviceWidget::hideControlsButtonClicked);
    m_widgetLayout->addWidget(m_hideControlsButton, 0, 0, Qt::AlignCenter);

    ui->widgetBox->setLayout(m_widgetLayout);

    adjust();
}

DeviceWidget::~DeviceWidget()
{
    delete ui;
}

void DeviceWidget::setAddress(int addr) {
    ui->idLabel->setText(QString("ID:%1").arg(addr));
}

// private methods

void DeviceWidget::paintEvent(QPaintEvent*) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void DeviceWidget::adjust() {
    ui->widgetBox->adjustSize();
    this->adjustSize();
    this->setMinimumSize(this->size());

    emit sizeChanged(this->size());
}

//void DeviceWidget::resizeEvent(QResizeEvent *event) {
//    qDebug() << "old size = " << event->oldSize() << "new size=" << this->size() << "minsize=" << this->minimumSize();
//}

//void DeviceWidget::setValue(quint16 reg, int value) {}

// private slots

void DeviceWidget::setLaserButton(quint16 value) {
    for(auto button : m_description.buttons) {
        if(button.name == "laser") {
            ui->laserButton->setChecked((value & button.mask) != 0);
        }
    }
}

void DeviceWidget::setTecButton(quint16 value) {
    for(auto button : m_description.buttons) {
        if(button.name == "tec") {
            ui->tecButton->setChecked((value & button.mask) != 0);
        }
    }
}

void DeviceWidget::laserButtonClicked(bool state) {
    for(auto button : m_description.buttons) {
        if(button.name == "laser") {
            if(m_commands.contains(button.code)) {
                auto cmd = m_commands.value(button.code);
                cmd->setFromWidget((state) ? button.onCommand : button.offCommand);
            }
            return;
        }
    }
}

void DeviceWidget::tecButtonClicked(bool state) {
    for(auto button : m_description.buttons) {
        if(button.name == "tec") {
            if(m_commands.contains(button.code)) {
                auto cmd = m_commands.value(button.code);
                cmd->setFromWidget((state) ? button.onCommand : button.offCommand);
            }
            return;
        }
    }
}

void DeviceWidget::hideControlsButtonClicked(bool flag) {
    m_hideControls = flag;
    qDebug() << "hideControlsButtonClicked" << flag;

    for(int idx = 1; idx < m_widgets.count(); ++idx) {
        int pinShift = idx-1;
        auto widget = m_widgets.at(idx);
        auto pinButton = m_pinButtons.value(pinShift, nullptr);
        if(m_hideControls) {
            if(!widget->isPinned()) {
                widget->setShown(false);
                if(pinButton != nullptr) pinButton->setVisible(false);
            }
        } else {
            if(!widget->isShown()) {
                widget->setShown(true);
                if(pinButton != nullptr) pinButton->setVisible(true);
            }
        }
    }
    adjust();
}

void DeviceWidget::pinButtonClicked(int idx, bool state) {
    qDebug() << "pinButtonClicked" << idx << state;
    if(idx < m_widgets.count()) {
        auto widget = m_widgets.at(idx);
        widget->setPinned(state);
    }
}
