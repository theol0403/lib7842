#include "controller.hpp"

namespace lib7842 {

void OdomController::driveToPoint(
  const Vector& targetPoint, double turnScale, const settleFunc_t& settleFunc) {
  resetPid();
  QAngle lastTarget = odometry->getState().theta;
  do {
    Vector closestPoint = closest(odometry->getState(), targetPoint);

    QAngle angleToClose = angleToPoint(closestPoint);
    if (std::isnan(angleToClose.convert(degree))) angleToClose = 0_deg;

    QLength distanceToClose = distanceToPoint(closestPoint);
    if (angleToClose.abs() >= 90_deg) distanceToClose = -distanceToClose;

    angleErr = angleToPoint(targetPoint);

    QLength distanceToTarget = distanceToPoint(targetPoint);

    if (distanceToTarget.abs() < pointRadius) {
      angleErr = 0_deg;
      distanceErr = distanceToClose;
    } else {
      angleErr = angleToPoint(targetPoint);
      lastTarget = angleErr + odometry->getState().theta;
      distanceErr = distanceToTarget;
    }

    angleErr = rollAngle90(angleErr);

    double angleVel = angleController->step(-angleErr.convert(degree));
    double distanceVel = distanceController->step(-distanceToClose.convert(millimeter));

    driveVector(distanceVel, angleVel * turnScale);
    pros::delay(10);
  } while (!settleFunc(this));

  driveVector(0, 0);
}

void OdomController::driveToPoint2(
  const Vector& targetPoint, double turnScale, const settleFunc_t& settleFunc) {
  resetPid();
  settleFunc_t exitFunc = makeSettle(pointRadius);
  do {
    angleErr = angleToPoint(targetPoint);
    distanceErr = distanceToPoint(targetPoint);

    if (angleErr.abs() > 90_deg) distanceErr = -distanceErr;
    angleErr = rollAngle90(angleErr);

    double angleVel = angleController->step(-angleErr.convert(degree));
    double distanceVel = distanceController->step(-distanceErr.convert(millimeter));

    driveVector(distanceVel, angleVel * turnScale);
    pros::delay(10);
  } while (!(exitFunc(this) || settleFunc(this)));

  driveDistanceAtAngle(
    distanceToPoint(targetPoint), angleCalc(angleToPoint(targetPoint)), turnScale, settleFunc);
  driveVector(0, 0);
}

} // namespace lib7842
