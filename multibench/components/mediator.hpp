#pragma once

#include <interfaces/pubsubinterface.h>

#include <QObject>
#include <model/event.hpp>

class Mediator final : public QObject {
  Q_OBJECT
 public:
  explicit Mediator(QObject* parent = nullptr);
  ~Mediator() = default;

  void Subscribe(const model::EventType eventType,
                 interfaces::Subscriber* subscriber);
  void LinkPublisher(interfaces::Publisher* publisher);
  void UnlinkPublisher(interfaces::Publisher* publisher);

 private slots:
  void Slot_PropagateEvent(const model::Event& event);

 private:
  std::unordered_multimap<const model::EventType, interfaces::Subscriber*>
      subscribers_;
};
