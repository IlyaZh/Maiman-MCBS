#include "mediator.hpp"

Mediator::Mediator(QObject* parent) : QObject{parent} {}

void Mediator::Subscribe(const model::EventType eventType,
                         interfaces::Subscriber* subscriber) {
  auto range = subscribers_.equal_range(eventType);
  if (std::any_of(range.first, range.second, [subscriber](const auto& item) {
        return item.second == subscriber;
      })) {
    return;
  }
  subscribers_.insert(std::make_pair(eventType, subscriber));
}

void Mediator::LinkPublisher(interfaces::Publisher* publisher) {
  connect(dynamic_cast<QObject*>(publisher),
          SIGNAL(Signal_PublishEvent(const model::Event&)), this,
          SLOT(Slot_PropagateEvent(const model::Event&)));
}

void Mediator::UnlinkPublisher(interfaces::Publisher* publisher) {
  disconnect(dynamic_cast<QObject*>(publisher),
             SIGNAL(Signal_PublishEvent(const model::Event&)), this,
             SLOT(Slot_PropagateEvent(const model::Event&)));
}

void Mediator::Slot_PropagateEvent(const model::Event& event) {
  const auto range = subscribers_.equal_range(event.type);
  for (auto it = range.first; it != range.second; it++) {
    auto& subscriber = it->second;
    subscriber->NewEvent(event);
  }
}
