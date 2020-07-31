#include "lib7842/api/odometry/settler.hpp"
#include "lib7842/api/odometry/odomController.hpp"
#include <iostream>

namespace lib7842 {

Settler&& Settler::distanceTo(const Vector& point, const QLength& trigger) {
  assembly.emplace([=]() { requirement(controller->distanceTo(point, trigger)); });
  return std::move(*this);
}

Settler&& Settler::angleTo(const Vector& point, const QAngle& trigger) {
  assembly.emplace([=]() { requirement(controller->angleTo(point, trigger)); });
  return std::move(*this);
}

Settler&& Settler::angleTo(const QAngle& angle, const QAngle& trigger) {
  assembly.emplace([=]() { requirement(controller->angleTo(angle, trigger)); });
  return std::move(*this);
}

Settler&& Settler::distanceErr(const QLength& trigger) {
  assembly.emplace([=]() { requirement(controller->distanceErr(trigger)); });
  return std::move(*this);
}

Settler&& Settler::angleErr(const QAngle& trigger) {
  assembly.emplace([=]() { requirement(controller->angleErr(trigger)); });
  return std::move(*this);
}

Settler&& Settler::distanceSettled() {
  assembly.emplace([=]() { requirement(controller->distanceSettled()); });
  return std::move(*this);
}

Settler&& Settler::turnSettled() {
  assembly.emplace([=]() { requirement(controller->turnSettled()); });
  return std::move(*this);
}

Settler&& Settler::angleSettled() {
  assembly.emplace([=]() { requirement(controller->angleSettled()); });
  return std::move(*this);
}

Settler&& Settler::distanceSettledUtil(const TimeUtil& timeUtil) {
  assembly.emplace([=]() { requirement(controller->distanceSettledUtil(timeUtil)); });
  return std::move(*this);
}

Settler&& Settler::angleSettledUtil(const TimeUtil& timeUtil) {
  assembly.emplace([=]() { requirement(controller->angleSettledUtil(timeUtil)); });
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

bool Settler::run(const OdomController* icontroller) {
  controller = icontroller;

  while (!assembly.empty()) {
    assembly.top()();
    assembly.pop();
  }

  auto error = controller->getDistanceError();
  auto change = error - lastError;
  lastError = error;

  if (driveAbort && driveAbort->isSettled(change.convert(millimeter))) {
    GLOBAL_WARN_S("Settler::run: Aborting drive command");
    return true;
  }
  return Trigger::run();
}

bool Settler::operator()(const OdomController* icontroller) {
  return run(icontroller);
}

void Settler::setDefaultAbort(const TimeUtil& itimeUtil) {
  defaultAbort = std::shared_ptr<SettledUtil>(itimeUtil.getSettledUtil());
}

std::shared_ptr<SettledUtil> Settler::defaultAbort {nullptr};

} // namespace lib7842
