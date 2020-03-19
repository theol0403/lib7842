#pragma once
#include "okapi/api/units/QAcceleration.hpp"
#include "okapi/api/units/QAngularSpeed.hpp"
#include "okapi/api/units/QSpeed.hpp"
#include "okapi/api/util/mathUtil.hpp"

namespace lib7842 {
using namespace okapi;

/**
 * Describes the kinematic limits of the robot. Used to customize the speeds of the robot when
 * following a path.
 */
struct PursuitLimits {
  /**
   * Pure Pursuit limits.
   *
   * @param iminVel   The minimum velocity through the entire path.
   * @param iaccel    The acceleration from min velocity to max velocity.
   * @param imaxVel   The maximum velocity.
   * @param idecel    The deceleration from max velocity to min velocity. Should be positive.
   * @param ifinalVel The final velocity at the end of the path. If lower than min velocity, the
   *                  robot will decelerate sooner but will not go slower than the min velocity.
   * @param ik        Optional. How much to slow down around turns. The velocity of the robot is
   *                  limited by this value divided by the path curvature. A higher curvature means
   *                  a lower velocity.
   */
  PursuitLimits(const QSpeed& iminVel, const QAcceleration& iaccel, const QSpeed& imaxVel,
                const QAcceleration& idecel, const QSpeed& ifinalVel,
                const std::optional<QSpeed>& ik = std::nullopt) :
    minVel(iminVel), accel(iaccel), maxVel(imaxVel), decel(idecel), finalVel(ifinalVel), k(ik) {}

  /**
   * Pure Pursuit limits. Deceleration and final velocity is inferred from acceleration and minimum
   * velocity.
   *
   * @param iminVel The minimum velocity through the entire path.
   * @param iaccel  The acceleration from min velocity to max velocity.
   * @param imaxVel The maximum velocity.
   * @param ik      Optional. How much to slow down around turns. The velocity of the robot is
   *                limited by this value divided by the path curvature. A higher curvature means a
   *                lower velocity.
   */
  PursuitLimits(const QSpeed& iminVel, const QAcceleration& iaccel, const QSpeed& imaxVel,
                const std::optional<QSpeed>& ik = std::nullopt) :
    PursuitLimits(iminVel, iaccel, imaxVel, iaccel, iminVel, ik) {}

  /**
   * Pure Pursuit limits. Time is used for acceleration and deceleration.
   *
   * @param iminVel   The minimum velocity through the entire path.
   * @param iaccel    The time to accelerate from min velocity to max velocity.
   * @param imaxVel   The maximum velocity.
   * @param idecel    The time to decelerate from max velocity to min velocity.
   * @param ifinalVel The final velocity at the end of the path. If lower than min velocity, the
   *                  robot will decelerate sooner but will not go slower than the min velocity.
   * @param ik        Optional. How much to slow down around turns. The velocity of the robot is
   *                  limited by this value divided by the path curvature. A higher curvature means
   *                  a lower velocity.
   */
  PursuitLimits(const QSpeed& iminVel, const QTime& iaccel, const QSpeed& imaxVel,
                const QTime& idecel, const QSpeed& ifinalVel,
                const std::optional<QSpeed>& ik = std::nullopt) :
    PursuitLimits(iminVel, (imaxVel - iminVel) / iaccel, imaxVel, (imaxVel - iminVel) / idecel,
                  ifinalVel, ik) {}

  /**
   * Pure Pursuit limits. Deceleration and final velocity is inferred from acceleration and minimum
   * velocity. Time is used for acceleration and deceleration.
   *
   * @param iminVel The minimum velocity through the entire path.
   * @param iaccel  The time to accelerate from min velocity to max velocity.
   * @param imaxVel The maximum velocity.
   * @param ik      Optional. How much to slow down around turns. The velocity of the robot is
   *                limited by this value divided by the path curvature. A higher curvature means a
   *                lower velocity.
   */
  PursuitLimits(const QSpeed& iminVel, const QTime& iaccel, const QSpeed& imaxVel,
                const std::optional<QSpeed>& ik = std::nullopt) :
    PursuitLimits(iminVel, (imaxVel - iminVel) / iaccel, imaxVel, ik) {}

  PursuitLimits(const QLength& iwheelDiam, const AbstractMotor::GearsetRatioPair& igearset,
                double imin, const QTime& iaccel, double imax, const QTime& idecel, double ifinal,
                const std::optional<QSpeed>& ik = std::nullopt) :
    PursuitLimits(iwheelDiam * (toUnderlyingType(igearset.internalGearset) * igearset.ratio * rpm) /
                    360_deg,
                  imin, iaccel, imax, idecel, ifinal, ik) {}

  /**
   * The minimum velocity through the entire path.
   */
  QSpeed minVel {0_mps};

  /**
   * The acceleration from min velocity to max velocity.
   */
  QAcceleration accel {0_mps2};

  /**
   * The maximum velocity.
   */
  QSpeed maxVel {0_mps};

  /**
   * The deceleration from max velocity to min velocity. Should be positive.
   */
  QAcceleration decel {0_mps2};

  /**
   * The final velocity at the end of the path. If lower than min velocity, the robot will
   * decelerate sooner but will not go slower than the min velocity.
   */
  QSpeed finalVel {0_mps};

  /**
   * How much to slow down around turns. The velocity of the robot is limited by this value divided
   * by the path curvature. A higher curvature means a lower velocity.
   */
  std::optional<QSpeed> k {std::nullopt};

private:
  PursuitLimits(const QSpeed& itopSpeed, double imin, const QTime& iaccel, double imax,
                const QTime& idecel, double ifinal, const std::optional<QSpeed>& ik) :
    PursuitLimits(itopSpeed * imin, iaccel, itopSpeed * imax, idecel, itopSpeed * ifinal, ik) {}
};
} // namespace lib7842
