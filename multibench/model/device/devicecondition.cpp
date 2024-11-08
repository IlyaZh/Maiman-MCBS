#include "devicecondition.h"

#include "device/commandsettings.h"
#include "devicewidget.h"

DeviceCondition::DeviceCondition(
    const QMap<quint16, QSharedPointer<CommandConverter>>& converters,
    const QVector<Led>& Leds, QLabel* label, QWidget* parent)
    : GuiWidgetBase(parent),
      m_converters(converters),
      m_Leds(Leds),
      m_Label(label) {
  for (int idleds = 0; idleds < m_Leds.count(); ++idleds) {
    auto led = m_Leds.at(idleds);
    for (int idled = 0; idled < led.ledMasks.count(); ++idled) {
      auto ledmask = led.ledMasks.at(idled);
      auto cmd = m_converters.value(ledmask.code);
      if (!m_codes.contains(ledmask.code)) m_codes.append(ledmask.code);
    }
  }
  //  QFont font12("Poppins", 12);
  //  font12.setLetterSpacing(QFont::PercentageSpacing, 105);
  //  font12.setPointSize(12);
  //  label->setFont(font12);
}

DeviceCondition::~DeviceCondition() {}

void DeviceCondition::setStateText(uint value, LedMask led) {
  QString msg = led.msg;
  if (led.code == 0x4 or led.code == 0x7A) msg = "";
  if ((value & led.mask) != 0) {
    if (!m_Label->text().contains(msg)) m_states.append(msg);
  } else {
    if (m_Label->text().contains(msg)) m_states.removeOne(msg);
  }
  m_Label->setText(m_states.join("; "));
}

void DeviceCondition::setData(quint16 code, quint16 data) {
  if (m_codes.contains(code)) {
    for (auto led : m_Leds) {
      for (auto ledMask : led.ledMasks) {
        if (ledMask.code == code) {
          QString msg = ledMask.msg;
          if (ledMask.code == 0x4 or ledMask.code == 0x7A) msg = "";
          if ((data & ledMask.mask) != 0) {
            if (!m_Label->text().contains(msg)) m_states.append(msg);
          } else {
            if (m_Label->text().contains(msg)) m_states.removeOne(msg);
          }
          m_Label->setText(m_states.join("; "));
        }
      }
    }
  }
}

QVector<quint16> DeviceCondition::Subscribe() { return m_codes; }

void DeviceCondition::updateStyle() {
  m_Label->setStyleSheet(StyleStorage::Device::condition());
  m_Label->update();
}
