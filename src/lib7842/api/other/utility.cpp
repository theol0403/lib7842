#include "utility.hpp"

namespace lib7842::util {

void driveVector(const std::shared_ptr<ChassisModel>& model, double forward, double yaw) {
  forward = std::clamp(forward, -1.0, 1.0);
  double leftOutput = forward + yaw;
  double rightOutput = forward - yaw;
  double maxInputMag = std::max(std::abs(leftOutput), std::abs(rightOutput));
  if (maxInputMag > 1.0) {
    leftOutput /= maxInputMag;
    rightOutput /= maxInputMag;
  }

  model->tank(leftOutput, rightOutput);
}

void strafeVector(const std::shared_ptr<XDriveModel>& model,
                  double forward,
                  double yaw,
                  double strafe) {
  forward = std::clamp(forward, -1.0, 1.0);
  strafe = std::clamp(strafe, -1.0, 1.0);
  double leftOutput = forward + yaw;
  double rightOutput = forward - yaw;
  double maxInputMag = std::max({std::abs(leftOutput), std::abs(rightOutput), std::abs(strafe)});
  if (maxInputMag > 1.0) {
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

void strafeVector(const std::shared_ptr<XDriveModel>& model,
                  double forward,
                  double yaw,
                  const QAngle& direction) {
  forward = std::clamp(forward, -1.0, 1.0);

  double scaleTopLeft = remapRange(std::sin((direction + 45_deg).convert(radian)), -0.70710678118,
                                   0.70710678118, -1.0, 1.0);
  double scaleTopRight = remapRange(std::cos((direction + 45_deg).convert(radian)), -0.70710678118,
                                    0.70710678118, -1.0, 1.0);

  double topLeft = forward * scaleTopLeft + yaw;
  double topRight = forward * scaleTopRight - yaw;
  double bottomLeft = forward * scaleTopRight + yaw;
  double bottomRight = forward * scaleTopLeft - yaw;

  double maxInputMag =
    std::max({std::abs(topLeft), std::abs(topRight), std::abs(bottomLeft), std::abs(bottomRight)});
  if (maxInputMag > 1.0) {
    topLeft /= maxInputMag;
    topRight /= maxInputMag;
    bottomLeft /= maxInputMag;
    bottomRight /= maxInputMag;
  }

  model->getTopLeftMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(topLeft, -1.0, 1.0) * model->getMaxVoltage()));
  model->getTopRightMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(topRight, -1.0, 1.0) * model->getMaxVoltage()));
  model->getBottomRightMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(bottomRight, -1.0, 1.0) * model->getMaxVoltage()));
  model->getBottomLeftMotor()->moveVoltage(
    static_cast<int16_t>(std::clamp(bottomLeft, -1.0, 1.0) * model->getMaxVoltage()));
}

} // namespace lib7842::util