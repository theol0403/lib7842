#pragma once
#include "okapi/api/units/QAcceleration.hpp"
#include "okapi/api/units/QSpeed.hpp"

namespace lib7842 {
using namespace okapi;

struct PursuitLimits {
  /**
   * Pure Pursuit limits. Deceleration and final velocity is inferred from acceleration and minimum
   * velocity.
   *
   * @param iminVel The minimum velocity of the robot throughout the entire path
   * @param iaccel  The maximum acceleration of the robot
   * @param imaxVel The maximum velocity of the robot
   * @param ik      Used to control how much to slow down around turns. The velocity of the robot is
   *                limited by this value divided by the path curvature. A higher curvature means a
   *                lower velocity.
   */
  PursuitLimits(const QSpeed& iminVel, const QAcceleration& iaccel, const QSpeed& imaxVel,
                const QSpeed& ik) :
    minVel(iminVel), accel(iaccel), maxVel(imaxVel), decel(iaccel), finalVel(iminVel), k(ik) {}

  /**
   * Pure Pursuit limits.
   *
   * @param iminVel   The minimum velocity of the robot throughout the entire path
   * @param iaccel    The maximum acceleration of the robot
   * @param imaxVel   The maximum velocity of the robot
   * @param idecel    The maximum deceleration of the robot
   * @param ifinalVel The final velocity of the robot at the end of the path. If lower than minimum
   *                  velocity, the robot will start decelerating sooner but will never reach this
   *                  velocity.
   * @param ik        Used to control how much to slow down around turns. The velocity of the robot
   *                  is limited by this value divided by the path curvature. A higher curvature
   *                  means a lower velocity.
   */
  PursuitLimits(const QSpeed& iminVel, const QAcceleration& iaccel, const QSpeed& imaxVel,
                const QAcceleration& idecel, const QSpeed& ifinalVel, const QSpeed& ik) :
    minVel(iminVel), accel(iaccel), maxVel(imaxVel), decel(idecel), finalVel(ifinalVel), k(ik) {}

  /**
   * The minimum velocity of the robot throughout the entire path
   */
  QSpeed minVel {0_mps};

  /**
   * The maximum acceleration of the robot
   */
  QAcceleration accel {0_mps2};

  /**
   * The maximum velocity of the robot
   */
  QSpeed maxVel {0_mps};

  /**
   * The maximum deceleration of the robot
   */
  QAcceleration decel {0_mps2};

  /**
   * The final velocity of the robot at the end of the path. If lower than minimum velocity, the
   * robot will start decelerating sooner but will never reach this velocity.
   */
  QSpeed finalVel {0_mps};

  /**
   * Used to control how much to slow down around turns. The velocity of the robot is limited by
   * this value divided by the path curvature. A higher curvature means a lower velocity.
   */
  QSpeed k {0_mps};
};
} // namespace lib7842
