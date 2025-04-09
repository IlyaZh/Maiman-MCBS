#ifndef DEVICEWIDGETDESC_H
#define DEVICEWIDGETDESC_H

#include <QtWidgets>

struct Content {
  QString fileName;
  QString description;
  QString link;
};

struct Control {
  quint16 min = 0;
  quint16 max = 0;
  quint16 value = 0;
  quint16 real = 0;
  QString name;
  bool fixed{false};
};

struct Limit {
  quint16 code;
  double minValue{0};
  double maxValue{0};
  quint16 minCode{0};
  quint16 maxCode{0};
  quint16 pairCode{0};
  bool isPairMin{true};
  QString name;
};

struct CalibrationKoef {
  quint16 code;
  double min;
  double max;
  QString name;
};

struct Checkbox {
  quint16 code = 0;
  quint16 onCommand = 0;
  quint16 offCommand = 0;
  quint16 mask = 0;
  QString name;
};

struct Button {
  QString name;
  quint16 code = 0;
  quint16 mask = 0;
  quint16 onCommand = 0;
  quint16 offCommand = 0;
};

struct LedMask {
  quint16 code = 0;
  quint16 mask = 0;
  QColor defaultColor;
  QColor maskColor;
  QString msg;
};

struct Led {
  QString name;
  QVector<LedMask> ledMasks;
};

struct DeviceWidgetDesc {
  uint id;
  QString name = "Unknown device";
  Content content;
  QVector<Limit> limits;
  QVector<CalibrationKoef> calibration;
  QVector<Control> controls;
  QVector<Checkbox> checkboxes;
  QMap<quint16, Button> buttons;
  QVector<Led> leds;
};
#endif  // DEVICEWIDGETDESC_H
