#include "trigger.hpp"

namespace lib7842 {

Trigger::Trigger(const OdomController* icontroller) : controller(icontroller) {}

void Trigger::isTrue(const std::function<bool()>& function) {
  requirements.emplace_back(function);
}

void Trigger::distanceTo(const Vector& point, const QLength& trigger) {
  isTrue([=] { return controller->distanceToPoint(point) < trigger; });
}

void Trigger::angleTo(const Vector& point, const QAngle& trigger) {
  isTrue([=] { return controller->angleToPoint(point) < trigger; });
}

void Trigger::angleTo(const QAngle& angle, const QAngle& trigger) {
  isTrue([=] { return (controller->getState().theta - angle).abs() < trigger; });
}

void Trigger::distanceErr(const QLength& trigger) {
  isTrue([=] { return controller->getDistanceError() < trigger; });
}

void Trigger::angleErr(const QAngle& trigger) {
  isTrue([=] { return controller->getAngleError() < trigger; });
}

void Trigger::distanceSettled() {
  isTrue([=] { return controller->isDistanceSettled(); });
}

void Trigger::turnSettled() {
  isTrue([=] { return controller->isTurnSettled(); });
}

void Trigger::angleSettled() {
  isTrue([=] { return controller->isAngleSettled(); });
}

void Trigger::maxTime(const QTime& time, const TimeUtil& timeUtil) {
  exeptions.emplace_back([=, timer = timeUtil.getTimer()]() mutable {
    timer->placeHardMark();
    return timer->getDtFromHardMark() > time;
  });
}

} // namespace lib7842