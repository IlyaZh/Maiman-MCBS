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
  WriteRequest(const QSet<quint8>& addresses, quint16 reg, quint16 value)
      : addresses_(addresses), reg_(reg), value_(value) {}
  const QSet<quint8> addresses_;
  const quint16 reg_;
  const quint16 value_;
};

struct SingleWriteRequest {
  SingleWriteRequest(quint8 address, quint16 reg, quint16 value)
      : address_(address), reg_(reg), value_(value) {}
  quint8 address_;
  quint16 reg_;
  quint16 value_;
};

struct StateUpdated {
  quint8 address;
  quint16 reg;
  QVector<quint16> values;
};

struct Answer {
  quint8 address;
  quint16 reg;
  quint16 value;
};
}  // namespace model::events::network
