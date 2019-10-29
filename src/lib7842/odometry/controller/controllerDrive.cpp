#include "controller.hpp"

namespace lib7842 {

using namespace lib7842::OdomMath;

void OdomController::moveDistanceAtAngle(
  const QLength& distance,
  const AngleCalculator& angleCalculator,
  double turnScale,
  const Settler& settler) {

  resetPid();
  auto lastTicks = model->getSensorVals();

  do {
    auto newTicks = model->getSensorVals();
    QLength leftDistance = ((newTicks[0] - lastTicks[0]) / odometry->getScales().straight) * meter;
    QLength rightDistance = ((newTicks[1] - lastTicks[1]) / odometry->getScales().straight) * meter;

    distanceErr = distance - ((leftDistance + rightDistance) / 2);
    angleErr = angleCalculator(this);

    double distanceVel = distanceController->step(-distanceErr.convert(millimeter));
    double angleVel = angleController->step(-angleErr.convert(degree));

    driveVector(distanceVel, angleVel * turnScale);
    pros::delay(10);
  } while (!settler(this));

  driveVector(0, 0);
}

void OdomController::moveDistance(const QLength& distance, const Settler& settler) {
  moveDistanceAtAngle(distance, makeAngleCalculator(odometry->getState().theta), 1, settler);
}

} // namespace lib7842
