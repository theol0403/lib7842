#include "lib7842/api/odometry/trigger.hpp"
#include "lib7842/api/odometry/odomController.hpp"

namespace lib7842 {

Trigger::Trigger(const OdomController* icontroller) : controller(icontroller) {}

Trigger&& Trigger::requirement(std::function<bool()>&& function) {
  requirements.emplace_back(function);
  return std::move(*this);
}

Trigger&& Trigger::exception(std::function<bool()>&& function) {
  exceptions.emplace_back(function);
  return std::move(*this);
}

Trigger&& Trigger::distanceTo(const Vector& point, const QLength& trigger) {
  return requirement([=] { return controller->distanceToPoint(point) < trigger; });
}

Trigger&& Trigger::angleTo(const Vector& point, const QAngle& trigger) {
  return requirement([=] { return controller->angleToPoint(point) < trigger; });
}

Trigger&& Trigger::angleTo(const QAngle& angle, const QAngle& trigger) {
  return requirement([=] { return (controller->getState().theta - angle).abs() < trigger; });
}

Trigger&& Trigger::distanceErr(const QLength& trigger) {
  return requirement([=] { return controller->getDistanceError() < trigger; });
}

Trigger&& Trigger::angleErr(const QAngle& trigger) {
  return requirement([=] { return controller->getAngleError() < trigger; });
}

Trigger&& Trigger::distanceSettled() {
  return requirement([=] { return controller->isDistanceSettled(); });
}

Trigger&& Trigger::turnSettled() {
  return requirement([=] { return controller->isTurnSettled(); });
}

Trigger&& Trigger::angleSettled() {
  return requirement([=] { return controller->isAngleSettled(); });
}

Trigger&& Trigger::distanceSettledUtil(const TimeUtil& timeUtil) {
  return requirement(
    [=, settledUtil = std::shared_ptr<SettledUtil>(timeUtil.getSettledUtil())]() mutable {
      return settledUtil->isSettled(controller->getDistanceError().convert(millimeter));
    });
}

Trigger&& Trigger::angleSettledUtil(const TimeUtil& timeUtil) {
  return requirement(
    [=, settledUtil = std::shared_ptr<SettledUtil>(timeUtil.getSettledUtil())]() mutable {
      return settledUtil->isSettled(controller->getAngleError().convert(degree));
    });
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

bool Trigger::operator()() {
  return run();
}

bool Trigger::run(const OdomController* icontroller) {
  controller = icontroller;
  return run();
}

bool Trigger::operator()(const OdomController* icontroller) {
  return run(icontroller);
}

} // namespace lib7842
