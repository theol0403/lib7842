#include "trigger.hpp"
#include "lib7842/api/odometry/odomController.hpp"

namespace lib7842 {

Trigger::Trigger(const OdomController* icontroller) : controller(icontroller) {}

Trigger&& Trigger::requirement(std::function<bool(const OdomController* icontroller)>&& function) {
  requirements.emplace_back(function);
  return std::move(*this);
}

using namespace std::placeholders; // for _1, _2, _3...

Trigger&& Trigger::requirement(std::function<bool()>&& function) {
  return requirement(
    [function = std::move(function)](const OdomController*) { return function(); });
}

Trigger&& Trigger::exception(std::function<bool(const OdomController* icontroller)>&& function) {
  exceptions.emplace_back(function);
  return std::move(*this);
}

Trigger&& Trigger::exception(std::function<bool()>&& function) {
  return exception([function = std::move(function)](const OdomController*) { return function(); });
}

Trigger&& Trigger::distanceTo(const Vector& point, const QLength& trigger) {
  return requirement([=](const OdomController* icontroller) {
    return icontroller->distanceToPoint(point) < trigger;
  });
}

Trigger&& Trigger::angleTo(const Vector& point, const QAngle& trigger) {
  return requirement(
    [=](const OdomController* icontroller) { return icontroller->angleToPoint(point) < trigger; });
}

Trigger&& Trigger::angleTo(const QAngle& angle, const QAngle& trigger) {
  return requirement([=](const OdomController* icontroller) {
    return (icontroller->getState().theta - angle).abs() < trigger;
  });
}

Trigger&& Trigger::distanceErr(const QLength& trigger) {
  return requirement(
    [=](const OdomController* icontroller) { return icontroller->getDistanceError() < trigger; });
}

Trigger&& Trigger::angleErr(const QAngle& trigger) {
  return requirement(
    [=](const OdomController* icontroller) { return icontroller->getAngleError() < trigger; });
}

Trigger&& Trigger::distanceSettled() {
  return requirement(
    [=](const OdomController* icontroller) { return icontroller->isDistanceSettled(); });
}

Trigger&& Trigger::turnSettled() {
  return requirement(
    [=](const OdomController* icontroller) { return icontroller->isTurnSettled(); });
}

Trigger&& Trigger::angleSettled() {
  return requirement(
    [=](const OdomController* icontroller) { return icontroller->isAngleSettled(); });
}

Trigger&& Trigger::distanceSettledUtil(const TimeUtil& timeUtil) {
  return requirement([=, settledUtil = std::shared_ptr<SettledUtil>(timeUtil.getSettledUtil())](
                       const OdomController* icontroller) mutable {
    return settledUtil->isSettled(icontroller->getDistanceError().convert(millimeter));
  });
}

Trigger&& Trigger::angleSettledUtil(const TimeUtil& timeUtil) {
  return requirement([=, settledUtil = std::shared_ptr<SettledUtil>(timeUtil.getSettledUtil())](
                       const OdomController* icontroller) mutable {
    return settledUtil->isSettled(icontroller->getAngleError().convert(degree));
  });
}

Trigger&& Trigger::maxTime(const QTime& time, const TimeUtil& timeUtil) {
  return exception([=, timer = std::shared_ptr<AbstractTimer>(timeUtil.getTimer())]() mutable {
    timer->placeHardMark();
    return timer->getDtFromHardMark() >= time;
  });
}

bool Trigger::run() {
  if (std::any_of(exceptions.begin(), exceptions.end(),
                  [=](const auto& function) { return function(controller); })) {
    return true;
  } else if (!requirements.empty() &&
             std::all_of(requirements.begin(), requirements.end(),
                         [=](const auto& function) { return function(controller); })) {
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