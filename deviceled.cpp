#include "deviceled.h"

// ---------------
// Device Led
// ---------------
const QString DeviceLed::LED_DEFAULT_COLOR = "#000000";
const QString DeviceLed::LED_DEFAULT_ACTIVE_COLOR = "#00ff00";

DeviceLed::DeviceLed(QString title, LedMask* maskObj, QString defaultColor, QWidget *parent)
    : QWidget(parent), m_title(title), m_defaultColor(QColor(defaultColor))
{
    m_masks.append(maskObj);
    commonConstructor();
}
DeviceLed::DeviceLed(QString title, QVector<LedMask*> masks, QString defaultColor, QWidget *parent)
    : QWidget(parent), m_title(title), m_masks(masks), m_defaultColor(QColor(defaultColor))
{
    commonConstructor();
}

DeviceLed::~DeviceLed() {
    for(auto *item : m_masks) {
        delete item;
    }
    m_masks.clear();
}

void DeviceLed::checkMasks() {
    bool hasOccuredMask = false;

    for(LedMask* mask : m_masks) {
        if(mask->maskOccured()) {
//            m_widget // set color
            hasOccuredMask = true;
            mask->activeColor();
        }
    }

    if(!hasOccuredMask) {
//        m_widget // set color
        m_defaultColor;
    }
}

void DeviceLed::commonConstructor() {
    m_widget = new QWidget(); // create a ledWidget
    // set a default color to m_widget
}

// ---------------
// Led Mask
// ---------------
LedMask::LedMask(DeviceLed* ledParent, DeviceCommand* command, QString message, quint16 mask, QString activeColor, QObject* parent)
    : QObject(parent), m_ledParent(ledParent), m_cmd(command), m_msg(message), m_mask(mask), m_activeColor(activeColor)
{
    m_maskOccured = false;
    if(m_cmd) connect(m_cmd, &DeviceCommand::valueChanged, [=]{
        this->m_maskOccured = ((this->m_cmd->getRawValue() & this->mask()) != 0);

    });
}
LedMask::~LedMask() {
    if(m_cmd) {
        m_cmd->disconnect();
    }
}
