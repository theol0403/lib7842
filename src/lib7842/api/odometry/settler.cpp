#include "settler.hpp"
#include "lib7842/api/odometry/odomController.hpp"

namespace lib7842 {

std::shared_ptr<SettledUtil> Settler::defaultAbort {nullptr};

void Settler::abort(const TimeUtil& itimeUtil) {
  driveAbort = std::shared_ptr<SettledUtil>(itimeUtil.getSettledUtil());
}

void Settler::noAbort() {
  driveAbort = nullptr;
}

bool Settler::operator()() {
  auto error = controller->getDistanceError();
  auto change = error - lastError;
  lastError = error;

  if (driveAbort->isSettled(change.convert(millimeter))) {
    return true;
  } else {
    return Trigger::operator()();
  }
}

void Settler::setDefaultAbort(const TimeUtil& itimeUtil) {
  defaultAbort = std::shared_ptr<SettledUtil>(itimeUtil.getSettledUtil());
}

} // namespace lib7842