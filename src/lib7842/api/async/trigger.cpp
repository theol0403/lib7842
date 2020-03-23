#include "lib7842/api/async/trigger.hpp"

namespace lib7842 {

Trigger&& Trigger::requirement(std::function<bool()>&& function) {
  requirements.emplace_back(std::move(function));
  return std::move(*this);
}

Trigger&& Trigger::needs(std::function<bool()>&& function) {
  return requirement(std::move(function));
}

Trigger&& Trigger::exception(std::function<bool()>&& function) {
  exceptions.emplace_back(std::move(function));
  return std::move(*this);
}

Trigger&& Trigger::unless(std::function<bool()>&& function) {
  return exception(std::move(function));
}

bool Trigger::run() {
  if (std::any_of(exceptions.begin(), exceptions.end(),
                  [](const auto& function) { return function(); })) {
    return true;
  } else if (!requirements.empty() &&
             std::all_of(requirements.begin(), requirements.end(),
                         [](const auto& function) { return function(); })) {
    return true;
  }
  return false;
}

bool Trigger::operator()() {
  return run();
}

std::function<bool()> Trigger::timePassed(const QTime& time) {
  return [=, timer = std::shared_ptr<AbstractTimer>(global::getTimeUtil()->getTimer())]() mutable {
    timer->placeHardMark();
    return timer->getDtFromHardMark() >= time;
  };
}

} // namespace lib7842
