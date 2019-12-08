#pragma once
#include "okapi/api/units/QAcceleration.hpp"
#include "okapi/api/units/QSpeed.hpp"

namespace lib7842 {
using namespace okapi;

struct PursuitLimits {
  /**
   * Pure Pursuit limits.
   *
   * @param iminVel The minimum velocity of the robot
   * @param imaxVel The maximum velocity of the robot
   * @param iaccel  The maximum acceleration of the robot
   * @param ik      Used to control how much to slow down around turns. The velocity of the robot is
   *                limited by this value divided by the path curvature. A higher curvature means a
   *                lower velocity. This value is usually best around 1, 0.5 slows down at any
   *                curvature in the path and 2 slows down around only a very sharp curvature.
   */
  PursuitLimits(const QSpeed& iminVel,
                const QSpeed& imaxVel,
                const QAcceleration& iaccel,
                const QSpeed& ik) :
    minVel(iminVel), maxVel(imaxVel), accel(iaccel), k(ik) {}

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
   * Used to control how much to slow down around turns. The velocity of the robot is limited by
   * this value divided by the path curvature. A higher curvature means a lower velocity. This value
   * is usually best around 1, 0.5 slows down at any curvature in the path and 2 slows down around
   * only a very sharp curvature.
   */
  QSpeed k {0_mps};
};

} // namespace lib7842