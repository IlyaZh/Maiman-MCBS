#include "mediator.h"

namespace components {

Mediator::Mediator(QObject* parent) : QObject{parent} {}

void Mediator::Subscribe(const model::EventType eventType,
                         interfaces::Subscriber* subscriber) {
  auto range = subscribers_.equal_range(eventType);
  if (std::any_of(range.first, range.second, [subscriber](const auto& item) {
        return item.second == subscriber;  // TODO
      })) {
    return;
  }
  subscribers_.insert(std::make_pair(eventType, subscriber));
}

void Mediator::LinkPublisher(QObject* publisher) {
  connect(publisher, SIGNAL(Signal_PublishEvent(model::Event)), this,
          SLOT(Slot_PropagateEvent(model::Event)));
}

void Mediator::UnlinkPublisher(QObject* publisher) {
  disconnect(dynamic_cast<QObject*>(publisher),
             SIGNAL(Signal_PublishEvent(const model::Event&)), this,
             SLOT(Slot_PropagateEvent(const model::Event&)));
}

void Mediator::Slot_PropagateEvent(const model::Event& event) {
  const auto range = subscribers_.equal_range(event.type_);
  for (auto it = range.first; it != range.second; it++) {
    auto& subscriber = it->second;
    subscriber->NewEvent(event);
  }
}

};  // namespace components
