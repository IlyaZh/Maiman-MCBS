#pragma once

#include <model/event.h>

namespace interfaces {

class Publisher {
 public:
  Publisher() = default;
  virtual ~Publisher();
  virtual void Signal_PublishEvent(model::Event&& event) = 0;
};

class Subscriber {
 public:
  Subscriber() = default;
  virtual ~Subscriber() = default;

  virtual void NewEvent(const model::Event& event) = 0;
};

}  // namespace interfaces

Q_DECLARE_INTERFACE(interfaces::Publisher, "interfaces::Publisher");
