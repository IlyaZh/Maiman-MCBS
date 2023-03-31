#include "connectionpolling.h"

#include <array>

namespace {
constexpr static const std::array<int, 6> kFibonachi = {1, 1, 2, 3, 5, 8};
}  // namespace
ConnectionPolling::ConnectionPolling(QObject *parent) : QObject(parent) {}

bool ConnectionPolling::isNeedAction() {
  if (kFibonachi.at(idx) == ++counter) {
    if (idx < kFibonachi.size() - 1) {
      ++idx;
    }
    counter = 0;
    return true;
  }
  return false;
}

void ConnectionPolling::reset() {
  idx = 0;
  counter = 0;
}
