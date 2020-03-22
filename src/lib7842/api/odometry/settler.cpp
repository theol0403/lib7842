#include "lib7842/api/odometry/settler.hpp"
#include "lib7842/api/odometry/odomController.hpp"
#include <iostream>

namespace lib7842 {

Settler&& Settler::distanceTo(const Vector& point, const QLength& trigger) {
  requirement([=] { return controller->distanceToPoint(point) < trigger; });
  return std::move(*this);
}

Settler&& Settler::angleTo(const Vector& point, const QAngle& trigger) {
  requirement([=] { return controller->angleToPoint(point) < trigger; });
  return std::move(*this);
}

Settler&& Settler::angleTo(const QAngle& angle, const QAngle& trigger) {
  requirement([=] { return (controller->getState().theta - angle).abs() < trigger; });
  return std::move(*this);
}

Settler&& Settler::distanceErr(const QLength& trigger) {
  requirement([=] { return controller->getDistanceError() < trigger; });
  return std::move(*this);
}

Settler&& Settler::angleErr(const QAngle& trigger) {
  requirement([=] { return controller->getAngleError() < trigger; });
  return std::move(*this);
}

Settler&& Settler::distanceSettled() {
  requirement([=] { return controller->isDistanceSettled(); });
  return std::move(*this);
}

Settler&& Settler::turnSettled() {
  requirement([=] { return controller->isTurnSettled(); });
  return std::move(*this);
}

Settler&& Settler::angleSettled() {
  requirement([=] { return controller->isAngleSettled(); });
  return std::move(*this);
}

Settler&& Settler::distanceSettledUtil(const TimeUtil& timeUtil) {
  requirement([=, settledUtil = std::shared_ptr<SettledUtil>(timeUtil.getSettledUtil())]() mutable {
    return settledUtil->isSettled(controller->getDistanceError().convert(millimeter));
  });
  return std::move(*this);
}

Settler&& Settler::angleSettledUtil(const TimeUtil& timeUtil) {
  requirement([=, settledUtil = std::shared_ptr<SettledUtil>(timeUtil.getSettledUtil())]() mutable {
    return settledUtil->isSettled(controller->getAngleError().convert(degree));
  });
  return std::move(*this);
}

Settler&& Settler::abort(const TimeUtil& itimeUtil) {
  driveAbort = std::shared_ptr<SettledUtil>(itimeUtil.getSettledUtil());
  return std::move(*this);
}

Settler&& Settler::noAbort() {
  driveAbort = nullptr;
  return std::move(*this);
}

void Settler::setDefaultAbort(const TimeUtil& itimeUtil) {
  defaultAbort = std::shared_ptr<SettledUtil>(itimeUtil.getSettledUtil());
}

std::shared_ptr<SettledUtil> Settler::defaultAbort {nullptr};

bool Settler::run(const OdomController* icontroller) {
  controller = icontroller;

  auto error = controller->getDistanceError();
  auto change = error - lastError;
  lastError = error;

  if (driveAbort && driveAbort->isSettled(change.convert(millimeter))) {
    GLOBAL_WARN_S("Settler::run: Aborting drive command");
    return true;
  } else {
    return Trigger::run();
  }
}

} // namespace lib7842
