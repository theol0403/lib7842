#include "trigger.hpp"

namespace lib7842 {

Trigger::Abort Trigger::defaultAbort = [](const OdomController*) {
  return false;
};

Trigger::Trigger(const OdomController* icontroller) : controller(icontroller) {}

void Trigger::distanceTo(const Vector& point, const QLength& trigger) {
  requirements.emplace_back([=] { return controller->distanceToPoint(point) < trigger; });
}

void Trigger::angleTo(const Vector& point, const QAngle& trigger) {
  requirements.emplace_back([=] { return controller->angleToPoint(point) < trigger; });
}

void Trigger::angleTo(const QAngle& angle, const QAngle& trigger) {
  requirements.emplace_back([=] { return (controller->getState().theta - angle).abs() < trigger; });
}

void Trigger::distanceErr(const QLength& trigger) {
  requirements.emplace_back([=] { return controller->getDistanceError() < trigger; });
}

void Trigger::angleErr(const QAngle& trigger) {
  requirements.emplace_back([=] { return controller->getAngleError() < trigger; });
}

void Trigger::distanceSettled() {
  requirements.emplace_back([=] { return controller->isDistanceSettled(); });
}

void Trigger::turnSettled() {
  requirements.emplace_back([=] { return controller->isTurnSettled(); });
}

void Trigger::angleSettled() {
  requirements.emplace_back([=] { return controller->isAngleSettled(); });
}

void Trigger::isTrue(const std::function<bool()>& function) {
  requirements.emplace_back(function);
}

void Trigger::maxTime(const QTime& time) {
  exeptions.emplace_back([=, timer = Timer()]() mutable {
    timer.placeHardMark();
    return timer.getDtFromHardMark() > time;
  });
}

} // namespace lib7842