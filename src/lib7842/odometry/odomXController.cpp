#include "odomXController.hpp"

namespace lib7842 {

using namespace lib7842::OdomMath;

OdomXController::OdomXController(
  const std::shared_ptr<XDriveModel>& imodel,
  const std::shared_ptr<CustomOdometry>& iodometry,
  std::unique_ptr<IterativePosPIDController> idistanceController,
  std::unique_ptr<IterativePosPIDController> iturnController,
  std::unique_ptr<IterativePosPIDController> iangleController,
  std::unique_ptr<IterativePosPIDController> istrafeController,
  const QLength& isettleRadius) :
  OdomController(
    imodel,
    iodometry,
    std::move(idistanceController),
    std::move(iturnController),
    std::move(iangleController),
    isettleRadius),
  model(imodel),
  strafeController(std::move(istrafeController)) {};

// /**
//  * Driving API
//  */

/**
 * Point API
 */
void OdomXController::driveToPoint(
  const Vector& targetPoint,
  const AngleCalculator& angleCalculator,
  double turnScale,
  const Settler& settler) {
  resetPid();
  do {
    const State& state = odometry->getState();
    Vector closestPoint = closest(state, targetPoint);
    Vector closestStrafe = closest(state, state.theta + 90_deg, targetPoint);

    QAngle angleToClose = angleToPoint(closestPoint);
    QAngle angleToTarget = angleCalculator(*this);
    QAngle angleToStrafe = rollAngle180(angleToPoint(closestStrafe) - 90_deg); // rotate angle

    QLength distanceToClose = distanceToPoint(closestPoint);
    QLength distanceToTarget = distanceToPoint(targetPoint);
    QLength distanceToStrafe = distanceToPoint(closestStrafe);

    // go backwards
    if (angleToClose.abs() >= 90_deg) distanceToClose = -distanceToClose;
    if (angleToStrafe.abs() >= 90_deg) distanceToStrafe = -distanceToStrafe;

    if (distanceToTarget.abs() < settleRadius) {
      angleErr = 0_deg;
      // used for settling
      distanceErr = distanceToClose;
    } else {
      angleErr = angleToTarget;
      // used for settling
      distanceErr = distanceToTarget;
    }

    // rotate angle to be +- 90
    angleErr = rollAngle90(angleErr);

    double angleVel = angleController->step(-angleErr.convert(degree));
    double distanceVel = distanceController->step(-distanceToClose.convert(millimeter));
    double strafeVel = strafeController->step(-distanceToStrafe.convert(millimeter));

    driveXVector(distanceVel, angleVel * turnScale, strafeVel);
    pros::delay(10);
  } while (!settler(*this));

  driveXVector(0, 0, 0);
}

void OdomXController::driveToPoint(const Vector& targetPoint, double turnScale, const Settler& settler) {
  driveToPoint(targetPoint, makeAngleCalculator(targetPoint), turnScale, settler);
}

bool OdomXController::defaultStrafeSettler(const OdomController& odom) {
  const OdomXController& xodom = dynamic_cast<const OdomXController&>(odom);
  return xodom.distanceController->isSettled() && xodom.strafeController->isSettled();
}

/**
 * OdomXController utilities
 */
void OdomXController::resetPid() {
  turnController->reset();
  distanceController->reset();
  angleController->reset();
  strafeController->reset();
  angleErr = 0_deg;
  distanceErr = 0_in;
}

void OdomXController::driveXVector(double forwardSpeed, double yaw, double strafe) {
  forwardSpeed = std::clamp(forwardSpeed, -1.0, 1.0);
  strafe = std::clamp(strafe, -1.0, 1.0);
  double leftOutput = forwardSpeed + yaw;
  double rightOutput = forwardSpeed - yaw;
  double maxInputMag = std::max(std::max(std::abs(leftOutput), std::abs(rightOutput)), std::abs(strafe));
  if (maxInputMag > 1) {
    leftOutput /= maxInputMag;
    rightOutput /= maxInputMag;
    strafe /= maxInputMag;
  }

  leftOutput = std::clamp(leftOutput, -1.0, 1.0);
  rightOutput = std::clamp(rightOutput, -1.0, 1.0);
  strafe = std::clamp(strafe, -1.0, 1.0);

  model->getTopLeftMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(leftOutput + strafe, -1.0, 1.0) * model->getMaxVoltage()));
  model->getTopRightMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(rightOutput - strafe, -1.0, 1.0) * model->getMaxVoltage()));
  model->getBottomRightMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(rightOutput + strafe, -1.0, 1.0) * model->getMaxVoltage()));
  model->getBottomLeftMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(leftOutput - strafe, -1.0, 1.0) * model->getMaxVoltage()));
}

} // namespace lib7842
