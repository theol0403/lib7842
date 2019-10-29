#include "controller.hpp"

namespace lib7842 {

using namespace lib7842::OdomMath;

/**
  * Odom Controller
  */
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
  * custom vector control which allows yaw to have priority over forwardSpeed
  * @param forwardSpeed
  * @param yaw
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
  * Relative Position Calcs
  */
QAngle OdomController::angleToPoint(const Vector& point) {
  QAngle angle = (std::atan2(
                    point.x.convert(inch) - odometry->getState().x.convert(inch),
                    point.y.convert(inch) - odometry->getState().y.convert(inch)) *
                  radian) -
                 odometry->getState().theta;
  return rollAngle180(angle);
}

QLength OdomController::distanceToPoint(const Vector& point) {
  return Vector::dist(odometry->getState(), point);
}

/**
  * Settle Functions
  */
Settler OdomController::makeSettler(const QAngle& angle) {
  return [=](OdomController* instance) {
    return that->angleErr.abs() < angle;
  };
}

Settler OdomController::makeSettler(const QLength& distance) {
  return [=](OdomController* instance) {
    return that->distanceErr.abs() < distance;
  };
}

Settler OdomController::makeSettler(const QLength& distance, const QAngle& angle) {
  return [=](OdomController* instance) {
    return that->distanceErr.abs() < distance && that->angleErr.abs() < angle;
  };
}

bool OdomController::defaultTurnSettler(OdomController* instance) {
  return that->turnController->isSettled();
}

bool OdomController::defaultDriveSettler(OdomController* instance) {
  return that->distanceController->isSettled() /*&& that->angleController->isSettled()*/;
}

} // namespace lib7842
