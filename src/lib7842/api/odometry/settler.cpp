#include "settler.hpp"
#include "lib7842/api/odometry/odomController.hpp"

namespace lib7842 {

std::function<bool()> Settler::defaultAbort = []() {
  return false;
};

void Settler::abort(const TimeUtil& itimeUtil) {
  driveAbort = [=, settledUtil =
                     std::shared_ptr<SettledUtil>(itimeUtil.getSettledUtil().release())]() mutable {
    return settledUtil->isSettled(controller->getDistanceError().convert(millimeter));
  };
}

void Settler::noAbort() {
  driveAbort = []() {
    return false;
  };
}

} // namespace lib7842