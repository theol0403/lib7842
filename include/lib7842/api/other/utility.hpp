#pragma once
#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
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
 * Control the chassis movement for an XDrive using voltage. Strafes at the given voltage in the
 * given direction. Applies magnitude control to prioritize turning. Range of forward, yaw, and
 * strafe is +-1, but yaw may be outside of the range which prioritizes turning.
 *
 * @param model     The chassis model
 * @param forward   The forward voltage
 * @param yaw       The yaw voltage
 * @param direction The direction
 */
void strafeVector(const std::shared_ptr<XDriveModel>& model, double forward, double yaw,
                  const QAngle& direction);

/**
 * Calculate the point along a given heading that is closest to a target point.
 *
 * @param  current The current point
 * @param  heading The heading
 * @param  target  The target point
 * @return the closest point
 */
Vector closest(const Vector& current, const QAngle& heading, const Vector& target);

/**
 * Calculate the point along a given heading that is closest to a target point. Uses the heading that is contained in State.
 *
 * @param  state  The current state with heading
 * @param  target The target point
 * @return the closest point
 */
Vector closest(const State& state, const Vector& target);

/**
 * Roll a given angle to be within the constraints of [0, 360] degrees. Does not change the actual
 * direction.
 *
 * @param  angle The input angle
 * @return The constrained angle
 */
QAngle rollAngle360(const QAngle& angle);

/**
 * Roll a given angle to be within the constraints of [-180, 180] degrees. Does not change the actual
 * direction.
 *
 * @param  angle The input angle
 * @return The constrained angle
 */
QAngle rollAngle180(const QAngle& angle);

/**
 * Rotate a given angle to be within the constraints of [-90, 90] degrees. Finds the nearest angle
 * parallel to another angle. Used to calculate angle for driving backwards.
 *
 * @param  angle The input angle
 * @return The rotated angle
 */
QAngle wrapAngle90(const QAngle& angle);

} // namespace lib7842::util
