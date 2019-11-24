#include "odomXController.hpp"

namespace lib7842 {

using namespace lib7842::OdomMath;

OdomXController::OdomXController(const std::shared_ptr<XDriveModel>& imodel,
                                 const std::shared_ptr<Odometry>& iodometry,
                                 std::unique_ptr<IterativePosPIDController> idistanceController,
                                 std::unique_ptr<IterativePosPIDController> iturnController,
                                 std::unique_ptr<IterativePosPIDController> iangleController) :
  OdomController(imodel,
                 iodometry,
                 std::move(idistanceController),
                 std::move(iturnController),
                 std::move(iangleController),
                 0_in),
  xModel(imodel) {};

void OdomXController::strafeRelativeDirection(const QLength& distance,
                                              const QAngle& direction,
                                              const AngleCalculator& angleCalculator,
                                              double turnScale,
                                              const Settler& settler) {
  State state = State(odometry->getState(StateMode::CARTESIAN));
  QAngle absoluteDirection = direction + state.theta;
  QLength x = sin(absoluteDirection.convert(radian)) * distance;
  QLength y = cos(absoluteDirection.convert(radian)) * distance;
  Vector target = Vector(state) + Vector(x, y);
  strafeToPoint(target, angleCalculator, turnScale, settler);
}

void OdomXController::strafeAbsoluteDirection(const QLength& distance,
                                              const QAngle& direction,
                                              const AngleCalculator& angleCalculator,
                                              double turnScale,
                                              const Settler& settler) {
  QLength x = sin(direction.convert(radian)) * distance;
  QLength y = cos(direction.convert(radian)) * distance;
  Vector target = Vector(State(odometry->getState(StateMode::CARTESIAN))) + Vector(x, y);
  strafeToPoint(target, angleCalculator, turnScale, settler);
}

void OdomXController::strafeToPoint(const Vector& targetPoint,
                                    const AngleCalculator& angleCalculator,
                                    double turnScale,
                                    const Settler& settler) {
  resetPid();
  do {
    distanceErr = distanceToPoint(targetPoint);
    angleErr = angleCalculator(*this);

    QAngle angleToTarget = angleToPoint(targetPoint);

    double distanceVel = distanceController->step(-distanceErr.convert(millimeter));
    double angleVel = angleController->step(-angleErr.convert(degree));

    strafeXVector(distanceVel, angleToTarget, angleVel * turnScale);
    pros::delay(10);
  } while (!settler(*this));

  driveXVector(0, 0, 0);
}

/**
 * OdomXController utilities
 */
void OdomXController::driveXVector(double speed, double yaw, double strafe) {
  speed = std::clamp(speed, -1.0, 1.0);
  strafe = std::clamp(strafe, -1.0, 1.0);
  double leftOutput = speed + yaw;
  double rightOutput = speed - yaw;
  double maxInputMag = std::max({std::abs(leftOutput), std::abs(rightOutput), std::abs(strafe)});
  if (maxInputMag > 1.0) {
    leftOutput /= maxInputMag;
    rightOutput /= maxInputMag;
    strafe /= maxInputMag;
  }

  leftOutput = std::clamp(leftOutput, -1.0, 1.0);
  rightOutput = std::clamp(rightOutput, -1.0, 1.0);
  strafe = std::clamp(strafe, -1.0, 1.0);

  xModel->getTopLeftMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(leftOutput + strafe, -1.0, 1.0) * model->getMaxVoltage()));
  xModel->getTopRightMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(rightOutput - strafe, -1.0, 1.0) * model->getMaxVoltage()));
  xModel->getBottomRightMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(rightOutput + strafe, -1.0, 1.0) * model->getMaxVoltage()));
  xModel->getBottomLeftMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(leftOutput - strafe, -1.0, 1.0) * model->getMaxVoltage()));
}

void OdomXController::strafeXVector(double speed, const QAngle& direction, double yaw) {
  speed = std::clamp(speed, -1.0, 1.0);

  double scaleTopLeft = remapRange(std::sin((direction + 45_deg).convert(radian)), -0.70710678118,
                                   0.70710678118, -1.0, 1.0);
  double scaleTopRight = remapRange(std::cos((direction + 45_deg).convert(radian)), -0.70710678118,
                                    0.70710678118, -1.0, 1.0);

  double topLeft = speed * scaleTopLeft + yaw;
  double topRight = speed * scaleTopRight - yaw;
  double bottomLeft = speed * scaleTopRight + yaw;
  double bottomRight = speed * scaleTopLeft - yaw;

  double maxInputMag =
    std::max({std::abs(topLeft), std::abs(topRight), std::abs(bottomLeft), std::abs(bottomRight)});
  if (maxInputMag > 1.0) {
    topLeft /= maxInputMag;
    topRight /= maxInputMag;
    bottomLeft /= maxInputMag;
    bottomRight /= maxInputMag;
  }

  xModel->getTopLeftMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(topLeft, -1.0, 1.0) * model->getMaxVoltage()));
  xModel->getTopRightMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(topRight, -1.0, 1.0) * model->getMaxVoltage()));
  xModel->getBottomRightMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(bottomRight, -1.0, 1.0) * model->getMaxVoltage()));
  xModel->getBottomLeftMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(bottomLeft, -1.0, 1.0) * model->getMaxVoltage()));
}

} // namespace lib7842
