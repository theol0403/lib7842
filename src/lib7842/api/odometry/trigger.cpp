#include "trigger.hpp"
#include "lib7842/api/odometry/odomController.hpp"

namespace lib7842 {

Trigger::Trigger(const OdomController* icontroller) : controller(icontroller) {}

void Trigger::requirement(std::function<bool()>&& function) {
  requirements.emplace_back(function);
}

void Trigger::exception(std::function<bool()>&& function) {
  exceptions.emplace_back(function);
}

void Trigger::distanceTo(const Vector& point, const QLength& trigger) {
  requirement([=] { return controller->distanceToPoint(point) < trigger; });
}

void Trigger::angleTo(const Vector& point, const QAngle& trigger) {
  requirement([=] { return controller->angleToPoint(point) < trigger; });
}

void Trigger::angleTo(const QAngle& angle, const QAngle& trigger) {
  requirement([=] { return (controller->getState().theta - angle).abs() < trigger; });
}

void Trigger::distanceErr(const QLength& trigger) {
  requirement([=] { return controller->getDistanceError() < trigger; });
}

void Trigger::angleErr(const QAngle& trigger) {
  requirement([=] { return controller->getAngleError() < trigger; });
}

void Trigger::distanceSettled() {
  requirement([=] { return controller->isDistanceSettled(); });
}

void Trigger::turnSettled() {
  requirement([=] { return controller->isTurnSettled(); });
}

void Trigger::angleSettled() {
  requirement([=] { return controller->isAngleSettled(); });
}

void Trigger::distanceSettledUtil(const TimeUtil& timeUtil) {
  requirement(
    [=, settledUtil = std::shared_ptr<SettledUtil>(timeUtil.getSettledUtil().release())]() mutable {
      return settledUtil->isSettled(controller->getDistanceError().convert(millimeter));
    });
}

void Trigger::angleSettledUtil(const TimeUtil& timeUtil) {
  requirement(
    [=, settledUtil = std::shared_ptr<SettledUtil>(timeUtil.getSettledUtil().release())]() mutable {
      return settledUtil->isSettled(controller->getAngleError().convert(degree));
    });
}

void Trigger::maxTime(const QTime& time, const TimeUtil& timeUtil) {
  exception([=, timer = std::shared_ptr<AbstractTimer>(timeUtil.getTimer().release())]() mutable {
    timer->placeHardMark();
    return timer->getDtFromHardMark() > time;
  });
}

bool Trigger::operator()() {
  if (std::any_of(exceptions.begin(), exceptions.end(),
                  [](const auto& function) { return function(); })) {
    return true;
  } else if (std::all_of(requirements.begin(), requirements.end(),
                         [](const auto& function) { return function(); })) {
    return true;
  }
  return false;
}

bool Trigger::operator()(const OdomController* icontroller) {
  controller = icontroller;
  return (*this)();
}

} // namespace lib7842