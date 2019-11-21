#pragma once

#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include "okapi/api/units/QAngle.hpp"

namespace lib7842::OdomMath {

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
 * @param current The current point
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
QAngle rotateAngle90(const QAngle& angle);

} // namespace lib7842::OdomMath
