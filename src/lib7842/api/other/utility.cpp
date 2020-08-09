#include "lib7842/api/other/utility.hpp"
#include "lib7842/api/positioning/point/mathPoint.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "okapi/api/util/mathUtil.hpp"

namespace lib7842::util {

void driveVector(const std::shared_ptr<ChassisModel>& model, double forward, double yaw,
                 motorMode mode) {
  forward = std::clamp(forward, -1.0, 1.0);
  double leftOutput = forward + yaw;
  double rightOutput = forward - yaw;
  double maxMag = std::max(std::abs(leftOutput), std::abs(rightOutput));
  if (maxMag > 1.0) {
    leftOutput /= maxMag;
    rightOutput /= maxMag;
  }

  if (mode == motorMode::voltage) {
    model->tank(leftOutput, rightOutput);
  } else {
    model->left(leftOutput);
    model->right(rightOutput);
  }
}

static void motorVoltage(AbstractMotor* motor, double pct,
                         const std::shared_ptr<XDriveModel>& imodel) {
  motor->moveVoltage(static_cast<int16_t>(std::clamp(pct, -1.0, 1.0) * imodel->getMaxVoltage()));
}

static void motorVelocity(AbstractMotor* motor, double pct,
                          const std::shared_ptr<XDriveModel>& imodel) {
  motor->moveVelocity(static_cast<int16_t>(std::clamp(pct, -1.0, 1.0) * imodel->getMaxVelocity()));
}

void strafeVector(const std::shared_ptr<XDriveModel>& model, double forward, double yaw,
                  const QAngle& direction, motorMode mode) {
  static const double sin45 = sin((45_deg).convert(radian));

  forward = std::clamp(forward, -1.0, 1.0);
  double wheelDirection = (direction + 45_deg).convert(radian);
  double scaleTopLeft = remapRange(std::sin(wheelDirection), -sin45, sin45, -1.0, 1.0);
  double scaleTopRight = remapRange(std::cos(wheelDirection), -sin45, sin45, -1.0, 1.0);

  double topLeft = forward * scaleTopLeft + yaw;
  double topRight = forward * scaleTopRight - yaw;
  double bottomLeft = forward * scaleTopRight + yaw;
  double bottomRight = forward * scaleTopLeft - yaw;

  double maxMag =
    std::max({std::abs(topLeft), std::abs(topRight), std::abs(bottomLeft), std::abs(bottomRight)});
  if (maxMag > 1.0) {
    topLeft /= maxMag;
    topRight /= maxMag;
    bottomLeft /= maxMag;
    bottomRight /= maxMag;
  }

  auto modeFnc = mode == motorMode::voltage ? motorVoltage : motorVelocity;

  modeFnc(model->getTopLeftMotor().get(), topLeft, model);
  modeFnc(model->getTopRightMotor().get(), topRight, model);
  modeFnc(model->getBottomRightMotor().get(), bottomRight, model);
  modeFnc(model->getBottomLeftMotor().get(), bottomLeft, model);
}

Vector closest(const Vector& current, const QAngle& heading, const Vector& target) {
  double headRad = heading.convert(radian);
  MathPoint n = MathPoint::normalize({sin(headRad), cos(headRad)});
  Vector v = target - current;
  double d = MathPoint::dot(v, n);
  return current + (n * d);
}

Vector closest(const State& state, const Vector& target) {
  return closest(state, state.theta, target);
}

QAngle rollAngle360(const QAngle& angle) {
  return angle - 360.0_deg * std::floor(angle.convert(degree) / 360.0);
}

QAngle wrapAngle90(const QAngle& angle) {
  QAngle iangle = rollAngle180(angle);
  if (iangle.abs() > 90_deg) {
    iangle += 180_deg;
    iangle = rollAngle180(iangle);
  }
  return iangle;
}

} // namespace lib7842::util
