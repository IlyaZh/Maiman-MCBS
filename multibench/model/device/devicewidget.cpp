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
#include "widgets/readparameterfactory.h"
#include "model/device/HiddenWidget.h"
#include "model/device/devicecondition.h"
#include <algorithm>

const QString DeviceWidget::linkStyleOn = "QLabel { \
        background: rgb(0,102,51); \
        border: 1px solid rgb(26,26,26); \
        border-radius: 3px; \
}";

const QString DeviceWidget::linkStyleOff = "QLabel { \
        background: rgb(175,0,0); \
        border: 1px solid rgb(26,26,26); \
        border-radius: 3px; \
}";

const QString DeviceWidget:: labelEnableStyle = "QLabel { \
        color: rgb(255,255,255); \
}";

const QString DeviceWidget::labelDisableStyle = "QLabel { \
    color: rgb(153,153,153); \
}";

DeviceWidget::DeviceWidget(const DeviceWidgetDesc& description, const QMap<quint16, QSharedPointer<DevCommand>>& commands, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceWidget),
    m_description(description),
    m_commands(commands),
    m_widgetLayout(new QGridLayout())
{
    ui->setupUi(this);

    ui->modelLabel->setText(QString("Model: %1").arg(m_description.name));
    setAddress(m_description.id);

    // Инициализация кнопки (Hide controls)
    auto m_hideControlsButton = new QPushButton(" "+tr("Hide controls"));
    m_hideControlsButton->setStyleSheet("border: 2px solid rgb(26,26,26);\nborder-radius: 3px;\nbackground: rgb(51,51,51);\ncolor: rgb(255,255,255);\n"
                                        "padding: 0px;\n"
                                        "margin-left: 10px;\n"
                                        "margin-bottom: 17px;");
    m_hideControlsButton->setCheckable(true);
    m_hideControlsButton->setChecked(false);
    m_hideControlsButton->setIconSize(QSize(10, 10));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/resources/images/hidecontrols-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon1.addFile(QString::fromUtf8(":/resources/images/showcontrols-icon.png"), QSize(), QIcon::Normal, QIcon::On);
    m_hideControlsButton->setIcon(icon1);
    m_hideControlsButton->setObjectName(QString::fromUtf8("hideControlButton"));
    m_hideControlsButton->setMinimumSize(QSize(125, 37));
    m_hideControlsButton->setMaximumSize(QSize(125, 37));
    QFont font1;
    font1.setFamily(QString::fromUtf8("Share Tech Mono"));
    m_hideControlsButton->setFont(font1);

    connect(m_hideControlsButton, &QPushButton::clicked, this, &DeviceWidget::hideControlsButtonClicked);
    m_widgetLayout->addWidget(m_hideControlsButton, 0, 0, Qt::AlignLeft);

    // Инициализация виджетов
    QVector<ReadParameterWidget*> readOnlyWidgets;
    for(const auto& control : description.controls) {
        auto valueCmd = m_commands.value(control.value, nullptr);
        auto maxCmd = m_commands.value(control.max, nullptr);
        auto minCmd = m_commands.value(control.min, nullptr);
        auto realCmd = m_commands.value(control.real, nullptr);

        if(realCmd != nullptr and valueCmd == nullptr) {
            // Обработка неизменяемых параметров
            readOnlyWidgets.append(ReadParameterFactory::createReadParameter(control.name, realCmd));
        } else {
            // Обработка изменяемых параметров
            auto hiddenWidget = new HiddenWidget(this);
            auto widget = new ControlWidget(control.name, valueCmd, maxCmd, minCmd, realCmd, hiddenWidget);
            hiddenWidget->layout()->setContentsMargins(10,0,10,0);
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
        hiddenWidget->layout()->setContentsMargins(10,12,10,0);
        //int maxUnitsLength=0;
        auto maxUnitsLengthIt = *std::max_element(std::begin(readOnlyWidgets),std::end(readOnlyWidgets),
                                                [=](ReadParameterWidget* widgetA,ReadParameterWidget* widgetB){
            return widgetA->getUnitslength()<widgetB->getUnitslength();
        });
        for(auto item : readOnlyWidgets) {
            item->setContentsMargins(0,0,0,4);
            item->setUnitsLength(maxUnitsLengthIt->getUnitslength());
            hiddenWidget->addWidget(item);
        }
        readOnlyWidgets.clear();
        m_widgetLayout->addWidget(hiddenWidget, 1, m_widgets.size(),Qt::AlignTop);
        m_widgets.append(hiddenWidget);
    }

    // Инициализация checkbox'ов
    QPointer<HiddenWidget> hiddenWidget;
    for(const auto &item : qAsConst(m_description.checkboxes)) {
        if(!hiddenWidget)
            hiddenWidget = new HiddenWidget(this);
            hiddenWidget->layout()->setContentsMargins(10,12,10,0);
        }
        auto cmd = m_commands.value(item.code, nullptr);
        if(cmd) {
            auto binaryWidget = new BinaryWidget(item, cmd, hiddenWidget);
            binaryWidget->setContentsMargins(0,0,0,4);
            hiddenWidget->addWidget(binaryWidget);
        }

    }
    if(hiddenWidget) {
        m_widgetLayout->addWidget(hiddenWidget.data(), 1, m_widgets.size(),Qt::AlignTop);
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
        pinButton->setMaximumWidth(36);
        pinButton->setStyleSheet(QString::fromUtf8("border: 2px solid rgb(26,26,26);\n"
                                                   "border-radius: 3px;\n"
                                                   "background: rgb(17,17,17);\n"
                                                   "color: rgb(255,255,255);\n"
                                                   "padding: 0px;\n"
                                                   "margin-right: 10px;\n"
                                                   "margin-left: 10px;"));
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
//    ui->laserButton->hide();
//    ui->tecButton->hide();
    for(const auto &button : qAsConst(m_description.buttons)) {
        QPointer<QPushButton> pButton;
        if(button.name.compare("laser", Qt::CaseInsensitive) == 0 && m_commands.contains(button.code)) {
            m_laserButton = new QPushButton("Laser", this);
            pButton = m_laserButton;
            connect(m_laserButton, &QPushButton::clicked, this, &DeviceWidget::laserButtonClicked);

            if(m_commands.contains(button.code)) {
                auto cmd = m_commands.value(button.code);
                connect(cmd.get(), &DevCommand::updatedValue, this, [this, cmd](){
                    setLaserButton(cmd->valueInt());
                });
            }


            /*ui->laserButton->setVisible(true);
            ui->laserButton->setChecked(false);
            connect(ui->laserButton, &QPushButton::clicked, this, &DeviceWidget::laserButtonClicked);

            if(m_commands.contains(button.code)) {
                auto cmd = m_commands.value(button.code);
                connect(cmd.get(), &DevCommand::updatedValue, this, [this, cmd](){
                    setLaserButton(cmd->valueInt());
                });
            }*/
        } else if (button.name.compare("tec", Qt::CaseInsensitive) == 0) {
            m_tecButton = new QPushButton("TEC", this);
            pButton = m_tecButton;
            m_tecButton->setVisible(true);
            connect(m_tecButton, &QPushButton::clicked, this, &DeviceWidget::tecButtonClicked);

            if(m_commands.contains(button.code)) {
                auto cmd = m_commands.value(button.code);
                connect(cmd.get(), &DevCommand::updatedValue, this, [this, cmd](){
                    setTecButton(cmd->valueInt());
                });
            }


            /*ui->tecButton->setVisible(true);
            connect(ui->tecButton, &QPushButton::clicked, this, &DeviceWidget::tecButtonClicked);
            ui->tecButton->setChecked(false);

            if(m_commands.contains(button.code)) {
                auto cmd = m_commands.value(button.code);
                connect(cmd.get(), &DevCommand::updatedValue, this, [this, cmd](){
                    setTecButton(cmd->valueInt());
                });
            }*/
        }

        if(pButton) {
            pButton->setMinimumSize(220, 36);
            pButton->setMaximumHeight(36);
            pButton->setFont(QFont("Share Tech Mono", 18));
            pButton->setStyleSheet("QPushButton \
                                   { \
                                       border: 2px solid rgb(26,26,26); \
                                       border-radius: 6px; \
                                       color: rgb(0,0,0); \
                                       background: rgb(189,0,0); \
                                   } \
                                   :checked \
                                   { \
                                       background: rgb(0,102,51); \
                                   }");
            pButton->setCheckable(true);
            pButton->setChecked(false);
            pButton->setVisible(true);
            ui->buttonsLayout->setContentsMargins(0,0,0,0);
            ui->buttonsLayout->addWidget(pButton);
            ui->buttonsLayout->setAlignment(pButton,Qt::AlignBottom);
        }
    }
    m_widgetLayout->setAlignment(Qt::AlignTop);
    //m_widgetLayout->setMargin(0);//6
    m_widgetLayout->setContentsMargins(0,2,0,0);//6,18,6,6
    m_widgetLayout->setSpacing(0);//9
    ui->widgetBox->setLayout(m_widgetLayout);
    m_hideControlsButton->setVisible(!m_widgets.isEmpty());

    m_condiotion = new DeviceCondition(m_commands,m_description.leds,ui->conditionLabel);
    //ui->widgetBox->setStyleSheet("QWidget { border: 1px solid red; }");
    adjust();

    //TODO:: label_2, для отображения строки состояния драйвера, команда 0700
}

DeviceWidget::~DeviceWidget()
{
    delete ui;
}

void DeviceWidget::setAddress(int addr) {
    ui->idLabel->setText(QString("ID:%1").arg(addr));
}

void DeviceWidget::setLink(bool link) {
    ui->linkLabel->setStyleSheet(link ? linkStyleOn : linkStyleOff);
    ui->idLabel->setStyleSheet(link ? labelEnableStyle : labelDisableStyle);
    ui->modelLabel->setStyleSheet(link ? labelEnableStyle : labelDisableStyle);
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

    //emit sizeChanged(this->size());
}

//void DeviceWidget::resizeEvent(QResizeEvent *event) {

//}

//void DeviceWidget::setValue(quint16 reg, int value) {}

// private slots

void DeviceWidget::setLaserButton(quint16 value) {
    for(const auto& button : qAsConst(m_description.buttons)) {
        if(button.name == "Laser") {
//            ui->laserButton->setChecked((value & button.mask) != 0);
            m_laserButton->setChecked((value & button.mask) != 0);
        }
    }
}

void DeviceWidget::setTecButton(quint16 value) {
    for(const auto& button : qAsConst(m_description.buttons)) {
        if(button.name == "Tec") {
//            ui->tecButton->setChecked((value & button.mask) != 0);
            m_tecButton->setChecked((value & button.mask) != 0);
        }
    }
}

void DeviceWidget::laserButtonClicked(bool state) {
    auto search = std::find_if(m_description.buttons.begin(), m_description.buttons.end(), [](const auto& button){
        return (button.name == "Laser");
    });
    if(search != m_description.buttons.end()) {
        auto laserButton = search.value();
        auto cmd = m_commands.value(laserButton.code, 0);
        if(cmd) {
            cmd->setFromWidget((state) ? laserButton.onCommand : laserButton.offCommand);
        }
    }
}

void DeviceWidget::tecButtonClicked(bool state) {
    auto search = std::find_if(m_description.buttons.begin(), m_description.buttons.end(), [](const auto& button){
        return (button.name == "Tec");
    });
    if(search != m_description.buttons.end()) {
        auto laserButton = search.value();
        auto cmd = m_commands.value(laserButton.code, 0);
        if(cmd) {
            cmd->setFromWidget((state) ? laserButton.onCommand : laserButton.offCommand);
        }
    }
}

void DeviceWidget::hideControlsButtonClicked(bool flag) {
    m_hideControls = flag;

    for(int idx = 1; idx < m_widgets.count(); ++idx) {
        int pinShift = idx-1;
        auto widget = m_widgets.at(idx);
        auto pinButton = m_pinButtons.value(pinShift, nullptr);
        auto topMargin = widget->layout()->contentsMargins().top();
        auto bottomMargin = widget->layout()->contentsMargins().bottom();
        if(m_hideControls) {
            if(!widget->isPinned()) {
                widget->setShown(false);
                widget->layout()->setContentsMargins(0,topMargin,0,bottomMargin);
                widget->layout()->setSpacing(0);
                if(pinButton != nullptr) pinButton->setVisible(false);
            }
        } else {
            if(!widget->isShown()) {
                widget->setShown(true);
                widget->layout()->setContentsMargins(10,topMargin,10,bottomMargin);
                widget->layout()->setSpacing(10);
                if(pinButton != nullptr) pinButton->setVisible(true);
            }
        }
    }
    adjust();
}

void DeviceWidget::pinButtonClicked(int idx, bool state) {
//    qDebug() << "pinButtonClicked" << idx << state;
    if(idx < m_widgets.count()) {
        auto widget = m_widgets.at(idx);
        widget->setPinned(state);
    }
}
