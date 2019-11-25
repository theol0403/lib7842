#pragma once

#include "okapi/api/units/QAcceleration.hpp"
#include "okapi/api/units/QSpeed.hpp"

namespace lib7842 {
using namespace okapi;

/**
 * Pure Pursuit limits.
 */
struct PursuitLimits {
  /**
   * The minimum velocity of the robot
   */
  QSpeed minVel {0_mps};

  /**
   * The maximum velocity of the robot
   */
  QSpeed maxVel {0_mps};

  /**
   * The minimum acceleration of the robot
   */
  QAcceleration accel {0_mps2};

  /**
   * How much to slow down around turns. This value is usually best around 0.5-2, 0.5 tends to slow
   * down around almost any curvature in the path, and 2 tends to slow down around only a very sharp
   * curvature.
   */
  double k {0};
};

} // namespace lib7842