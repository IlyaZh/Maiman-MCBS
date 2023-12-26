#pragma once

#include <QVariant>
#include <unordered_map>

#include "events/network.h"

namespace model {

enum class EventType { kReadDevice, kWriteDevice, kDeviceStateUpdated };

inline QString ToString(const EventType& type) {
  static const std::unordered_map<EventType, QString> map{
      {EventType::kReadDevice, "ReadDevice"},
      {EventType::kWriteDevice, "WriteDevice"},
      {EventType::kDeviceStateUpdated, "DeviceStateUpdated"}};

  //  if (const auto it = map.find(type); it != map.cend()) {
  //    return it->second;
  //  }
  throw std::runtime_error(
      QString("Trying to use ToString with unexpected type: %1")
          .arg(static_cast<std::size_t>(type))
          .toStdString());
}

template <typename T>
struct ParseTo {
  T t_;
};

inline EventType Parse(const QString& type, const ParseTo<EventType>&) {
  //  static const std::unordered_map<QString, EventType> map{
  //      {"ReadDevice", EventType::kReadDevice},
  //      {"WriteDevice", EventType::kWriteDevice},
  //      {"DeviceStateUpdated", EventType::kDeviceStateUpdated}};

  //  if (const auto it = map.find(type); it != map.cend()) {
  //    return it->second;
  //  }
  throw std::runtime_error(
      QString("Trying to use Parse with unexpected arg: %1")
          .arg(type)
          .toStdString());
}

namespace events {

using Types = std::variant<network::ReadRequest, network::WriteRequest,
                           network::StateUpdated, network::Answer,
                           network::SingleWriteRequest, network::DataRequest>;

}  // namespace events

struct Event {
  Event(const EventType& type, events::Types&& data)
      : type_(type), data_(data) {}
  Event(const EventType& type, const events::Types& data)
      : type_(type), data_(data) {}
  const EventType type_{};
  const events::Types data_;
};

}  // namespace model
