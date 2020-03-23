#include "lib7842/api/odometry/settler.hpp"
#include "lib7842/api/odometry/odomController.hpp"
#include <iostream>

namespace lib7842 {

Settler&& Settler::distanceTo(const Vector& point, const QLength& trigger) {
  assembly.emplace_back([=]() { return controller->distanceTo(point, trigger); });
  return std::move(*this);
}

Settler&& Settler::angleTo(const Vector& point, const QAngle& trigger) {
  assembly.emplace_back([=]() { return controller->angleTo(point, trigger); });
  return std::move(*this);
}

Settler&& Settler::angleTo(const QAngle& angle, const QAngle& trigger) {
  assembly.emplace_back([=]() { return controller->angleTo(angle, trigger); });
  return std::move(*this);
}

Settler&& Settler::distanceErr(const QLength& trigger) {
  assembly.emplace_back([=]() { return controller->distanceErr(trigger); });
  return std::move(*this);
}

Settler&& Settler::angleErr(const QAngle& trigger) {
  assembly.emplace_back([=]() { return controller->angleErr(trigger); });
  return std::move(*this);
}

Settler&& Settler::distanceSettled() {
  assembly.emplace_back([=]() { return controller->distanceSettled(); });
  return std::move(*this);
}

Settler&& Settler::turnSettled() {
  assembly.emplace_back([=]() { return controller->turnSettled(); });
  return std::move(*this);
}

Settler&& Settler::angleSettled() {
  assembly.emplace_back([=]() { return controller->angleSettled(); });
  return std::move(*this);
}

Settler&& Settler::distanceSettledUtil(const TimeUtil& timeUtil) {
  assembly.emplace_back([=]() { return controller->distanceSettledUtil(timeUtil); });
  return std::move(*this);
}

Settler&& Settler::angleSettledUtil(const TimeUtil& timeUtil) {
  assembly.emplace_back([=]() { return controller->angleSettledUtil(timeUtil); });
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

  std::for_each(assembly.begin(), assembly.end(),
                [&](const auto& assembler) { requirements.emplace_back(assembler()); });

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
