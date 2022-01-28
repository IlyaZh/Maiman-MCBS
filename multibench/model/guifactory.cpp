#include "guifactory.h"
#include <QDebug>
#include "widgets/plusminuswidget.h"
#include "widgets/calibrationdialog.h"
#include <algorithm>

class DevCommand;

GuiFactory::GuiFactory(const QString& fileName, QObject *parent) :
    QObject(parent),
    m_fileName(fileName)
{

}


void GuiFactory::start() {
    if(!m_parseWorker.isNull()) m_parseWorker->deleteLater();
    m_parseWorker = new ParserWorker(m_fileName, ParserType::XmlParser);
    m_thread = new QThread();
    m_parseWorker->moveToThread(m_thread);

    connect(m_thread, SIGNAL(started()), m_parseWorker, SLOT(process()));
    connect(m_parseWorker, SIGNAL(finished()), m_thread, SLOT(quit()));
    connect(m_parseWorker, SIGNAL(finished()), this, SLOT(parsingFinished()));
    connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));
    connect(m_parseWorker, SIGNAL(errorOccured(QString)), this, SLOT(threadError(QString)));
    connect(m_parseWorker, SIGNAL(errorOccured(QString)), m_thread, SLOT(quit()));
    connect(m_parseWorker, SIGNAL(errorOccured(QString)), m_thread, SLOT(deleteLater()));
    m_thread->start();
}

DeviceWidget* GuiFactory::createDeviceWidget(quint16 id, const QMap<quint16, QSharedPointer<DevCommand>>& commands) {
    if(m_deviceWidgets.contains(id)) {
        return new DeviceWidget(m_deviceWidgets[id], commands);
    }
    return nullptr;
}

CalibrationDialog* GuiFactory::createCalibrationDialog(quint16 id, const QMap<quint16, QSharedPointer<DevCommand>> &commands){
    if(m_deviceWidgets.contains(id)) {
        return new CalibrationDialog(m_deviceWidgets[id],commands);
    }
    return nullptr;
}

// private slots

void GuiFactory::parsingFinished() {
    m_parseWorker->disconnect();
    TreeItem* parserTree = m_parseWorker->data();
    if(parseTree(*parserTree)) {
        qDebug() << "Gui Parse tree is ok!";
    } else {
        qDebug() << "Gui Can't parse tree";
    }
    m_parseWorker->deleteLater();
    delete parserTree;
}

void GuiFactory::threadError(const QString &msg) {
    qDebug() << "Error while parsing. " << msg;
}

// private methods

bool GuiFactory::parseTree(const TreeItem& tree) {
    if(tree.name() == "GUI") {
        for(int i = 0; i < tree.childCount(); ++i) {
            const TreeItem& device = tree.child(i);
            QString name = device.name();
            if(name == "Device") {
                auto devWidget = parseDevice(device);
                m_deviceWidgets.insert(devWidget.id, devWidget);
            }
        }
        return true;
    }
    return false;
}

DeviceWidgetDesc GuiFactory::parseDevice(const TreeItem& item) {
    DeviceWidgetDesc widgetDesc;
    for(int c = 0; c < item.childCount(); ++c) {
        const TreeItem& devOption = item.child(c);

        if(devOption.name() == "id") {
            widgetDesc.id = devOption.value().toString().toUInt(nullptr, 16);
        } else if(devOption.name() == "name") {
            widgetDesc.name = devOption.value().toString();
        } else if (devOption.name() == "Content") {
            widgetDesc.content = parseDeviceContent(devOption);
        } else if (devOption.name() == "Limits") {
            for(int l = 0; l < devOption.childCount(); ++l) {
                const TreeItem& limit = devOption.child(l);
                widgetDesc.limits << parseDeviceLimit(limit);
            }
        } else if (devOption.name() == "CalibrationKoefs") {
            for(int i = 0; i < devOption.childCount(); ++i) {
                const TreeItem& koef = devOption.child(i);
                widgetDesc.calibration << parseCalibrationKoef(koef);
            }
        } else if (devOption.name() == "ParamControls") {
            for(int i = 0; i < devOption.childCount(); ++i) {
                const TreeItem& ctrl = devOption.child(i);
                widgetDesc.controls << parseParamControls(ctrl);
            }

        } else if (devOption.name() == "Checkboxes") {
            for(int i = 0; i < devOption.childCount(); ++i) {
                const TreeItem& checkbox = devOption.child(i);
                widgetDesc.checkboxes << parseCheckboxes(checkbox);
            }

        } else if (devOption.name() == "Buttons") {
            for(int i = 0; i < devOption.childCount(); ++i) {
                const TreeItem& buttonItem = devOption.child(i);
                auto button = parseButtons(buttonItem);
                widgetDesc.buttons.insert(button.code, button);
            }
        } else if (devOption.name() == "Leds") {
            for(int i = 0; i < devOption.childCount(); ++i) {
                const TreeItem& led = devOption.child(i);
                widgetDesc.leds << parseLeds(led);
            }
        }
    }

    std::stable_sort(widgetDesc.controls.begin(), widgetDesc.controls.end(), [](const Control& left, const Control& right){
        Q_UNUSED(right)
        uint16_t lCmd, rCmd;
        if(left.value != 0) {
            lCmd = left.value;
        } else if(left.real != 0) {
            lCmd = left.real;
        } else {
            return false;
        }

        if(right.value != 0) {
            rCmd = right.value;
        } else if (right.real != 0) {
            rCmd = right.real;
        } else {
            return true;
        }

        return lCmd < rCmd;
    });

    // Ищем виджет Current и ставим его на первое место
    std::stable_sort(widgetDesc.controls.begin(), widgetDesc.controls.end(), [](const Control& left, const Control& right){
        Q_UNUSED(right)
        return (left.fixed);
    });
    return widgetDesc;
}

Content GuiFactory::parseDeviceContent(const TreeItem& item) {
    Content content;
    for(int i = 0; i <item.childCount(); ++i) {
        const TreeItem& subContent = item.child(i);
        if(subContent.name() == "Image") {
            content.fileName = subContent.value().toString();
        } else if (subContent.name() == "Description") {
            content.description = subContent.value().toString();
        } else if (subContent.name() == "Link") {
            content.link = subContent.value().toString();
        }
    }
    return content;
}

Limit GuiFactory::parseDeviceLimit(const TreeItem& item) {
    Limit limit;
    limit.name = item.value().toString();
    for(int i = 0; i < item.childCount(); ++i) {
        const TreeItem& arg = item.child(i);
        if(arg.name() == "limitCode") {
            limit.code = static_cast<quint16>(arg.value().toString().toUInt(nullptr, 16));
        } else if (arg.name() == "minValue") {
            limit.minValue = arg.value().toDouble();
        } else if (arg.name() == "maxValue") {
            limit.maxValue = arg.value().toDouble();
        } else if(arg.name() == "minCode") {
            limit.minCode = static_cast<quint16>(arg.value().toString().toUInt(nullptr, 16));
        } else if (arg.name() == "maxCode") {
            limit.maxCode = static_cast<quint16>(arg.value().toString().toUInt(nullptr, 16));
        } else if (arg.name() == "pair") {
            limit.pairCode = static_cast<quint16>(arg.value().toString().toUInt(nullptr, 16));
        } else if (arg.name() == "pairType") {
            limit.isPairMin = (arg.value().toString().toLower() == QString("min"));
        }
    }
    return limit;
}

CalibrationKoef GuiFactory::parseCalibrationKoef(const TreeItem& item) {
    CalibrationKoef koef;
    koef.name = item.value().toString();
    for(int i = 0; i < item.childCount(); ++i) {
        const TreeItem& arg = item.child(i);
        if(arg.name() == "code") {
            koef.code = static_cast<quint16>(arg.value().toString().toUInt(nullptr, 16));
        } else if(arg.name() == "min") {
            koef.min = arg.value().toDouble();
        } else if(arg.name() == "max") {
            koef.max = arg.value().toDouble();
        }
    }
    return koef;
}

Control GuiFactory::parseParamControls(const TreeItem& item) {
    Control control;
    control.name = item.value().toString();
    for(int i = 0; i < item.childCount(); ++i) {
        const TreeItem& arg = item.child(i);
        if(arg.name() == "min") {
            control.min = static_cast<quint16>(arg.value().toString().toUInt(nullptr, 16));
        } else if(arg.name() == "max") {
            control.max = static_cast<quint16>(arg.value().toString().toUInt(nullptr, 16));
        } else if(arg.name() == "value") {
            control.value = static_cast<quint16>(arg.value().toString().toUInt(nullptr, 16));
        } else if(arg.name() == "real") {
            control.real = static_cast<quint16>(arg.value().toString().toUInt(nullptr, 16));
        } else if(arg.name() == "fixed") {
            control.fixed = true;
        }
    }
    return control;
}

Checkbox GuiFactory::parseCheckboxes(const TreeItem& item) {
    Checkbox checkbox;
    checkbox.name = item.value().toString();
    for(int i = 0; i < item.childCount(); ++i) {
        const TreeItem& arg = item.child(i);
        if(arg.name() == "code") {
            checkbox.code = static_cast<quint16>(arg.value().toString().toUInt(nullptr, 16));
        } else if(arg.name() == "onCommand") {
            checkbox.onCommand = static_cast<quint16>(arg.value().toString().toUInt(nullptr, 16));
        } else if(arg.name() == "offCommand") {
            checkbox.offCommand = static_cast<quint16>(arg.value().toString().toUInt(nullptr, 16));
        } else if(arg.name() == "mask") {
            checkbox.mask = static_cast<quint16>(arg.value().toString().toUInt(nullptr, 16));
        }

    }
    return checkbox;
}

Button GuiFactory::parseButtons(const TreeItem& item) {
    Button button;
    button.name = item.value().toString();
    for(int i = 0; i < item.childCount(); ++i) {
        const TreeItem& btnItem = item.child(i);
        if(btnItem.name() == "mask") {
            button.mask = static_cast<quint16>(btnItem.value().toString().toUInt(nullptr, 16));
        } else if(btnItem.name() == "code") {
            button.code = static_cast<quint16>(btnItem.value().toString().toUInt(nullptr, 16));
        } else if(btnItem.name() == "onCommand") {
            button.onCommand = static_cast<quint16>(btnItem.value().toString().toUInt(nullptr, 16));
        } else if(btnItem.name() == "offCommand") {
            button.offCommand = static_cast<quint16>(btnItem.value().toString().toUInt(nullptr, 16));
        }
    }
    return button;
}

Led GuiFactory::parseLeds(const TreeItem& item) {
    Led led;
    for(int i = 0; i < item.childCount(); ++i) {
        const TreeItem& ledTag = item.child(i);
        if(ledTag.name() == "label") {
            led.name = ledTag.value().toString();
        } else if (ledTag.name() == "LedMask") {
            LedMask ledMask;
            ledMask.msg = ledTag.value().toString();
            for(int j = 0; j < ledTag.childCount(); ++j) {
                const TreeItem& ledMaskTag = ledTag.child(j);
                if(ledMaskTag.name() == "code") {
                    ledMask.code = static_cast<quint16>(ledMaskTag.value().toString().toUInt(nullptr, 16));
                } else if (ledMaskTag.name() == "mask") {
                    ledMask.mask = static_cast<quint16>(ledMaskTag.value().toString().toUInt(nullptr, 16));
                } else if (ledMaskTag.name() == "maskColor") {
                    ledMask.maskColor.setNamedColor(ledMaskTag.value().toString());
                } else if (ledMaskTag.name() == "defaultColor") {
                    ledMask.defaultColor.setNamedColor(ledMaskTag.value().toString());
                }
            }
            led.ledMasks << ledMask;
        }
    }
    return led;
}

bool GuiFactory::hasLimits(quint16 id){
    return(m_deviceWidgets.contains(id) and !m_deviceWidgets[id].calibration.isEmpty());
}

bool GuiFactory::hasCalibration(quint16 id){
    return(m_deviceWidgets.contains(id) and !m_deviceWidgets[id].limits.isEmpty());
}
