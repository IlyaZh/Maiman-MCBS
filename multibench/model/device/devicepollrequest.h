#pragma once

#include <QObject>

struct DevicePollRequest {
  explicit DevicePollRequest(quint8 addr = 0, quint16 code = 0,
                             quint8 count = 0, uint interval = 1)
      : code(code),
        addr(addr),
        count(count),
        interval(interval),
        counter(interval - 1) {}
  bool isRequestReady() {
    ++counter;
    if (counter == interval) {
      counter = 0;
      return true;
    }
    return false;
  }
  const quint16 code;
  const quint8 addr;
  const quint16 count;
  const uint interval;

 private:
  uint counter;
};
