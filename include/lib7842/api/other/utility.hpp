#pragma once
#include "okapi/api/chassis/model/chassisModel.hpp"
#include "okapi/api/chassis/model/xDriveModel.hpp"
#include "okapi/api/units/QAngle.hpp"
#include <memory>

namespace lib7842::util {

using namespace okapi;

/**
 * Returns the sign of the given number. Returns -1 if the number is negative, 1 if positive, 0
 * otherwise.
 *
 * @param  val The value
 */
template <typename T> int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

/**
 * Control the chassis movement using voltage. Applies magnitude control to prioritize turning.
 * Range of forward and yaw is +-1, but yaw may be outside of the range which prioritizes turning.
 *
 * @param model   The chassis model
 * @param forward The forward voltage
 * @param yaw     The yaw voltage
 */
void driveVector(const std::shared_ptr<ChassisModel>& model, double forward, double yaw);

/**
 * Control the chassis movement for an XDrive using voltage. Applies magnitude control to prioritize
 * turning. Range of forward, yaw, and strafe is +-1, but yaw may be outside of the range which
 * prioritizes turning.
 *
 * @param model   The chassis model
 * @param forward The forward voltage
 * @param yaw     The yaw voltage
 * @param strafe  The strafe voltage
 */
void strafeVector(const std::shared_ptr<XDriveModel>& model,
                  double forward,
                  double yaw,
                  double strafe);

/**
 * Control the chassis movement for an XDrive using voltage. Strafes at the given voltage in the
 * given direction. Applies magnitude control to prioritize turning. Range of forward, yaw, and
 * strafe is +-1, but yaw may be outside of the range which prioritizes turning.
 *
 * @param model     The chassis model
 * @param forward   The forward voltage
 * @param yaw       The yaw voltage
 * @param direction The direction
 */
void strafeVector(const std::shared_ptr<XDriveModel>& model,
                  double forward,
                  double yaw,
                  const QAngle& direction);

} // namespace lib7842::util