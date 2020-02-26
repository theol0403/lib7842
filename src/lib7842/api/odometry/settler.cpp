#include "settler.hpp"
#include "lib7842/api/odometry/odomController.hpp"

namespace lib7842 {

std::shared_ptr<SettledUtil> Settler::defaultAbort {nullptr};

Settler::Settler(Trigger&& trigger) : Trigger(std::move(trigger)) {}

Settler&& Settler::abort(const TimeUtil& itimeUtil) {
  driveAbort = std::shared_ptr<SettledUtil>(itimeUtil.getSettledUtil());
  return std::move(*this);
}

Settler&& Settler::noAbort() {
  driveAbort = nullptr;
  return std::move(*this);
}

bool Settler::run() {
  auto error = controller->getDistanceError();
  auto change = error - lastError;
  lastError = error;

  if (driveAbort && driveAbort->isSettled(change.convert(millimeter))) {
    std::cerr << "Settler::run: ERROR: Aborting drive command" << std::endl;
    return true;
  } else {
    return Trigger::run();
  }
}

void Settler::setDefaultAbort(const TimeUtil& itimeUtil) {
  defaultAbort = std::shared_ptr<SettledUtil>(itimeUtil.getSettledUtil());
}

} // namespace lib7842