#include "lib7842/api/odometry/trigger.hpp"
#include "lib7842/api/odometry/odomController.hpp"

namespace lib7842 {

Trigger&& Trigger::requirement(std::function<bool()>&& function) {
  requirements.emplace_back(function);
  return std::move(*this);
}

Trigger&& Trigger::exception(std::function<bool()>&& function) {
  exceptions.emplace_back(function);
  return std::move(*this);
}

Trigger&& Trigger::maxTime(const QTime& time) {
  return exception(
    [=, timer = std::shared_ptr<AbstractTimer>(global::getTimeUtil()->getTimer())]() mutable {
      timer->placeHardMark();
      return timer->getDtFromHardMark() >= time;
    });
}

Trigger&& Trigger::noAbort() {
  return std::move(*this);
};

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

bool Trigger::operator()(const OdomController* icontroller) {
  return run(icontroller);
}

bool Trigger::run(const OdomController* icontroller) {
  controller = icontroller;
  return run();
}

} // namespace lib7842
