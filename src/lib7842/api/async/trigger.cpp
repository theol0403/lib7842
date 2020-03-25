#include "lib7842/api/async/trigger.hpp"

namespace lib7842 {

Trigger::Function Trigger::Function::operator!()&& {
  return std::not_fn(std::move(*this));
}

Trigger&& Trigger::requirement(Trigger::Function&& function) {
  requirements.emplace_back(std::move(function));
  return std::move(*this);
}

Trigger&& Trigger::require(Trigger::Function&& function) {
  return requirement(std::move(function));
}

Trigger&& Trigger::exception(Trigger::Function&& function) {
  exceptions.emplace_back(std::move(function));
  return std::move(*this);
}

Trigger&& Trigger::unless(Trigger::Function&& function) {
  return exception(std::move(function));
}

bool Trigger::run() {
  if (std::any_of(exceptions.begin(), exceptions.end(),
                  [](const auto& function) { return function(); })) {
    return true;
  }
  if (!requirements.empty() && std::all_of(requirements.begin(), requirements.end(),
                                           [](const auto& function) { return function(); })) {
    return true;
  }
  return false;
}

bool Trigger::operator()() {
  return run();
}

Trigger::Function Trigger::time(const QTime& time) {
  return [=, timer = std::shared_ptr<AbstractTimer>(global::getTimeUtil()->getTimer())]() mutable {
    timer->placeHardMark();
    return timer->getDtFromHardMark() >= time;
  };
}

} // namespace lib7842
