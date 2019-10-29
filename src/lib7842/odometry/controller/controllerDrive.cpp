#include "controller.hpp"

namespace lib7842 {

void OdomController::driveDistanceAtAngle(
  const QLength& distance,
  const AngleCalculator& turnCalc,
  double turnScale,
  const Settler& settleFunc) {

  resetPid();
  auto lastTicks = model->getSensorVals();

  do {
    auto newTicks = model->getSensorVals();
    QLength leftDistance = ((newTicks[0] - lastTicks[0]) / odometry->getScales().straight) * meter;
    QLength rightDistance = ((newTicks[1] - lastTicks[1]) / odometry->getScales().straight) * meter;

    distanceErr = distance - ((leftDistance + rightDistance) / 2);
    angleErr = turnCalc(this);

    double distanceVel = distanceController->step(-distanceErr.convert(millimeter));
    double angleVel = angleController->step(-angleErr.convert(degree));

    driveVector(distanceVel, angleVel * turnScale);
    pros::delay(10);
  } while (!settleFunc(this));

  driveVector(0, 0);
}

void OdomController::driveDistance(const QLength& distance, const Settler& settleFunc) {
  driveDistanceAtAngle(distance, angleCalc(odometry->getState().theta), 1, settleFunc);
}

} // namespace lib7842
