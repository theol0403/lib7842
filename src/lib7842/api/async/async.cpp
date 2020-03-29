#include "lib7842/api/async/async.hpp"
#include "pros/rtos.hpp"

namespace lib7842 {

Async::Async(Trigger::Function&& iaction) : action(std::move(iaction)) {
  startTask("Async");
}

Async::Async(Trigger&& itrigger, Trigger::Function&& iaction) :
  trigger(std::move(itrigger)), action(std::move(iaction)) {
  startTask("Async");
}

void Async::forceStart() {
  _forceStart = true;
}

void Async::forceStop() {
  stopTask();
  _complete = true;
}

bool Async::hasStarted() const {
  return _started;
}

bool Async::isComplete() const {
  return _complete;
}

void Async::waitUntilComplete() const {
  while (!_complete) {
    pros::delay(10);
  }
}

void Async::loop() {
  while (!(trigger() || _forceStart)) {
    pros::delay(10);
  }
  _started = true;
  action();
  _complete = true;
}

} // namespace lib7842
