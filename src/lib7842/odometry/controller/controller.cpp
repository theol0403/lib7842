#include "odomController.hpp"

namespace lib7842 {

using namespace lib7842::OdomMath;

OdomController::OdomController(
  std::shared_ptr<ChassisModel> imodel,
  std::shared_ptr<CustomOdometry> iodometry,
  std::unique_ptr<IterativePosPIDController> idistanceController,
  std::unique_ptr<IterativePosPIDController> iturnController,
  std::unique_ptr<IterativePosPIDController> iangleController) :
  model(std::move(imodel)),
  odometry(std::move(iodometry)),
  distanceController(std::move(idistanceController)),
  angleController(std::move(iangleController)),
  turnController(std::move(iturnController)),
  pointRadius(1_ft) {};

/**
 * Turning API
 */
void OdomController::turn(
  const AngleCalculator& angleCalculator, const Turner& turner, const Settler& settler) {
  resetPid();
  do {
    angleErr = angleCalculator(*this);
    double vel = turnController->step(-angleErr.convert(degree));
    turner(*model, vel);
    pros::delay(10);
  } while (!settler(*this));
  turner(*model, 0);
}

void OdomController::turnToAngle(
  const QAngle& angle, const Turner& turner, const Settler& settler) {
  turn(makeAngleCalculator(angle), turner, settler);
}

void OdomController::turnAngle(const QAngle& angle, const Turner& turner, const Settler& settler) {
  turn(makeAngleCalculator(angle + odometry->getState().theta), turner, settler);
}

void OdomController::turnToPoint(
  const Vector& point, const Turner& turner, const Settler& settler) {
  turn(makeAngleCalculator(point), turner, settler);
}

/**
 * Driving API
 */
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
    angleErr = angleCalculator(*this);

    double distanceVel = distanceController->step(-distanceErr.convert(millimeter));
    double angleVel = angleController->step(-angleErr.convert(degree));

    driveVector(distanceVel, angleVel * turnScale);
    pros::delay(10);
  } while (!settler(*this));

  driveVector(0, 0);
}

void OdomController::moveDistance(const QLength& distance, const Settler& settler) {
  moveDistanceAtAngle(distance, makeAngleCalculator(odometry->getState().theta), 1, settler);
}

/**
 * Point API
 */
void OdomController::driveToPoint(
  const Vector& targetPoint, double turnScale, const Settler& settler) {
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
  } while (!settler(*this));

  driveVector(0, 0);
}

void OdomController::driveToPoint2(
  const Vector& targetPoint, double turnScale, const Settler& settler) {
  resetPid();
  Settler exitFunc = makeSettler(pointRadius);
  do {
    angleErr = angleToPoint(targetPoint);
    distanceErr = distanceToPoint(targetPoint);

    if (angleErr.abs() > 90_deg) distanceErr = -distanceErr;
    angleErr = rollAngle90(angleErr);

    double angleVel = angleController->step(-angleErr.convert(degree));
    double distanceVel = distanceController->step(-distanceErr.convert(millimeter));

    driveVector(distanceVel, angleVel * turnScale);
    pros::delay(10);
  } while (!(exitFunc(*this) || settler(*this)));

  moveDistanceAtAngle(
    distanceToPoint(targetPoint), makeAngleCalculator(angleToPoint(targetPoint)), turnScale,
    settler);
  driveVector(0, 0);
}

/**
 * Default Settlers
 */
bool OdomController::defaultTurnSettler(const OdomController& odom) {
  return odom.turnController->isSettled();
}

bool OdomController::defaultDriveSettler(const OdomController& odom) {
  return odom.distanceController->isSettled() /*&& odom.angleController->isSettled()*/;
}

/**
 * Default Turners
 */
void OdomController::pointTurn(ChassisModel& model, double vel) {
  model.rotate(vel);
}

void OdomController::leftPivot(ChassisModel& model, double vel) {
  model.left(vel * 2);
}

void OdomController::rightPivot(ChassisModel& model, double vel) {
  model.right(-vel * 2);
}

/**
 * Settler Generators
 */
Settler OdomController::makeSettler(const QAngle& angle) {
  return [=](const OdomController& odom) {
    return odom.angleErr.abs() < angle;
  };
}

Settler OdomController::makeSettler(const QLength& distance) {
  return [=](const OdomController& odom) {
    return odom.distanceErr.abs() < distance;
  };
}

Settler OdomController::makeSettler(const QLength& distance, const QAngle& angle) {
  return [=](const OdomController& odom) {
    return odom.distanceErr.abs() < distance && odom.angleErr.abs() < angle;
  };
}

/**
 * AngleCalculator Generators
 */
AngleCalculator OdomController::makeAngleCalculator(const QAngle& angle) {
  QAngle iangle = rollAngle180(angle);
  return [=](const OdomController& odom) {
    return rollAngle180(iangle - odom.odometry->getState().theta);
  };
}

AngleCalculator OdomController::makeAngleCalculator(const Vector& point) {
  return [=](const OdomController& odom) {
    return odom.angleToPoint(point);
  };
}

AngleCalculator OdomController::makeAngleCalculator() {
  return [=](const OdomController&) {
    return 0_deg;
  };
}

/**
 * OdomController utilities
 */
void OdomController::driveVector(double forwardSpeed, double yaw) {
  double leftOutput = forwardSpeed + yaw;
  double rightOutput = forwardSpeed - yaw;
  double maxInputMag = std::max(std::abs(leftOutput), std::abs(rightOutput));
  if (maxInputMag > 1) {
    leftOutput /= maxInputMag;
    rightOutput /= maxInputMag;
  }

  model->tank(leftOutput, rightOutput);
}

void OdomController::resetPid() {
  turnController->reset();
  distanceController->reset();
  angleController->reset();
  angleErr = 0_deg;
  distanceErr = 0_in;
}

/**
  * Position Calculations
  */
QAngle OdomController::angleToPoint(const Vector& point) const {
  const State& state = odometry->getState();
  Vector diff = point - state;
  QAngle angle = (std::atan2(diff.x.convert(meter), diff.y.convert(meter)) * radian) - state.theta;
  return rollAngle180(angle);
}

QLength OdomController::distanceToPoint(const Vector& point) const {
  return Vector::dist(odometry->getState(), point);
}

} // namespace lib7842
