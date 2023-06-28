#pragma once

#include <QVariant>
#include <unordered_map>

#include "events/network.h"

namespace model {

enum class EventType { kNone, kReadDevice, kWriteDevice, kDeviceStateUpdated };

inline QString ToString(const EventType& type) {
  static const std::unordered_map<EventType, QString> map{
      {EventType::kNone, "None"},
      {EventType::kReadDevice, "ReadDevice"},
      {EventType::kWriteDevice, "WriteDevice"},
      {EventType::kDeviceStateUpdated, "DeviceStateUpdated"}};

  if (const auto it = map.find(type); it != map.cend()) {
    return it->second;
  }
  throw std::runtime_error(
      QString("Trying to use ToString with unexpected type: %1")
          .arg(static_cast<std::size_t>(type))
          .toStdString());
}

namespace events {

using Types = std::variant<network::ReadRequest, network::WriteRequest,
                           network::StateUpdated>;

}  // namespace events

struct Event {
  EventType type{};
  events::Types data;
};

}  // namespace model
