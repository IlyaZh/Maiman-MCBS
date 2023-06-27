#pragma once

#include <QtCore>
#include <QtGlobal>

namespace model::events::network {

struct ReadRequest {
  quint8 address;
  quint16 reg;
  quint8 count;
};

struct WriteRequest {
  quint8 address;
  quint16 reg;
  quint16 value;
};

struct StateUpdated {
  quint8 address;
  quint16 reg;
  QVector<quint16> values;
};

}  // namespace model::events::network
