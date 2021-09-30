#include "devicewidget.h"
#include "ui_devicewidget.h"
#include <QDebug>

//#include "model/device/parameterwidget.h"
#include "device/devicecommand.h"
#include <QStyleOption>
#include <QPainter>
#include "widgets/binarywidget.h"
#include "widgets/commandwidget.h"
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
    for(auto control : description.controls) {
        qDebug() << control.max << control.min << control.name << control.real << control.value;
        auto valueCmd = m_commands.value(control.value, nullptr);
        auto maxCmd = m_commands.value(control.max, nullptr);
        auto minCmd = m_commands.value(control.min, nullptr);
        auto realCmd = m_commands.value(control.real, nullptr);

        if(realCmd != nullptr and valueCmd == nullptr) {
            // Обработка неизменяемых параметров
            readOnlyWidgets.append(new ReadParameterWidget(control, realCmd));
        } else {
            // Обработка изменяемых параметров
            auto widget = new CommandWidget(control, valueCmd, maxCmd, minCmd, realCmd, this);
            auto hiddenWidget = new HiddenWidget();
            m_widgetLayout->addWidget(hiddenWidget, 1, m_widgets.size());
            hiddenWidget->addWidget(widget);
            if(control.name.toLower() == "current")
                hiddenWidget->setPinned(true);
            m_widgets.append(hiddenWidget);

            // TODO: Добавь везде в конструтор тут сигналы и слоты нужные
            connect(widget, &CommandWidget::valueChanged, this, &DeviceWidget::parameterChanged);
        }
    }
    // Закидываем неизменяемые параметры в виджет
    if(readOnlyWidgets.count() > 0) {
        auto hiddenWidget = new HiddenWidget();
        for(auto item : readOnlyWidgets) {
            hiddenWidget->addWidget(item);
            // TODO: добавь слоты для прихода значений
        }
        readOnlyWidgets.clear();
        m_widgetLayout->addWidget(hiddenWidget, 1, m_widgets.size());
        m_widgets.append(hiddenWidget);
    }

    // Инициализация checkbox'ов
    QPointer<HiddenWidget> hiddenWidget;
    for(auto item : m_description.checkboxes) {
        if(!hiddenWidget)
            hiddenWidget = new HiddenWidget();

        auto widget = new BinaryWidget(item);
        hiddenWidget->addWidget(widget);

        // TODO: Добавь везде в конструтор тут сигналы и слоты нужные
        connect(widget, &BinaryWidget::valueChanged, this, &DeviceWidget::checkBoxClicked);

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
    for(auto button: description.buttons) {
        if(button.name == "laser") {
            ui->laserButton->setVisible(true);
            ui->laserButton->setChecked(false);
            connect(ui->laserButton, &QPushButton::clicked, this, &DeviceWidget::laserButtonClicked);
        } else if (button.name == "tec") {
            ui->tecButton->setVisible(true);
            connect(ui->tecButton, &QPushButton::clicked, this, &DeviceWidget::tecButtonClicked);
            ui->tecButton->setChecked(false);
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
    qDebug() << "device widget size=" << this->size() << this->minimumSize() << m_widgetLayout->sizeHint() << ui->widgetBox->size() << ui->widgetBox->sizeHint();

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
    qDebug() << "setValue DeviceWidget DELETE";
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

void DeviceWidget::adjust() {
    ui->widgetBox->adjustSize();
    this->adjustSize();
    this->setMinimumSize(this->size());

    emit sizeChanged();
}

//void DeviceWidget::setValue(quint16 reg, int value) {}

// private slots

void DeviceWidget::laserButtonClicked(bool state) {
    for(auto button : m_description.buttons) {
        if(button.name == "laser") {
            emit dataChanged(button.code, (state) ? button.onCommand : button.offCommand);
            return;
        }
    }
}

void DeviceWidget::tecButtonClicked(bool state) {
    for(auto button : m_description.buttons) {
        if(button.name == "tec") {
            emit dataChanged(button.code, (state) ? button.onCommand : button.offCommand);
            return;
        }
    }
}

void DeviceWidget::commandChanged(quint16 reg) {
    if(m_commands.contains(reg)) {
        auto cmd = m_commands.value(reg);
        qDebug() << "DeviceWidget" << reg << cmd->valueDouble();

        if(m_description.buttons.contains(reg)) {
            auto button = m_description.buttons.value(reg);
            if(button.name == "laser") {
                ui->laserButton->setChecked((cmd->valueUInt() & button.mask) != 0);
            } else if(button.name == "tec") {
                ui->tecButton->setChecked((cmd->valueUInt() & button.mask) != 0);
            }
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

void DeviceWidget::checkBoxClicked(quint16 cmd, quint8 bit, bool state) {
    qDebug() << "checkBoxClicked" << cmd << bit << state;
}
