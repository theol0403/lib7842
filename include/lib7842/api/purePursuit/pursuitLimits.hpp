#pragma once
#include "okapi/api/units/QAcceleration.hpp"
#include "okapi/api/units/QAngularSpeed.hpp"
#include "okapi/api/units/QSpeed.hpp"
#include "okapi/api/util/mathUtil.hpp"

namespace lib7842 {
using namespace okapi;

/**
 * Pure Pursuit limits. Describes the kinematic limits of the robot. Used to customize the speeds of
 * the robot when following a path.
 */
struct PursuitLimits {
  /**
   * Full limits.
   *
   * @param iminVel   The minimum velocity through the entire path.
   * @param iaccel    The acceleration from min velocity to max velocity.
   * @param imaxVel   The maximum velocity.
   * @param idecel    The deceleration from max velocity to final velocity. Should be positive.
   * @param ifinalVel The final velocity at the end of the path. If lower than min velocity, the
   *                  robot will decelerate sooner but will not go slower than the min velocity.
   * @param ik        Optional. How much to slow down around turns. The velocity of the robot is
   *                  limited by this value divided by the path curvature. A higher curvature means
   *                  a lower velocity.
   */
  PursuitLimits(const QSpeed& iminVel, const QAcceleration& iaccel, const QSpeed& imaxVel,
                const QAcceleration& idecel, const QSpeed& ifinalVel,
                const std::optional<QSpeed>& ik = std::nullopt);
  /**
   * Deceleration and final velocity is inferred from acceleration and minimum velocity.
   *
   * @param iminVel The minimum velocity through the entire path.
   * @param iaccel  The acceleration from min velocity to max velocity.
   * @param imaxVel The maximum velocity.
   * @param ik      Optional. How much to slow down around turns. The velocity of the robot is
   *                limited by this value divided by the path curvature. A higher curvature means a
   *                lower velocity.
   */
  PursuitLimits(const QSpeed& iminVel, const QAcceleration& iaccel, const QSpeed& imaxVel,
                const std::optional<QSpeed>& ik = std::nullopt);
  /**
   * Time is used for acceleration and deceleration.
   *
   * @param iminVel   The minimum velocity through the entire path.
   * @param iaccel    The time to accelerate from min velocity to max velocity.
   * @param imaxVel   The maximum velocity.
   * @param idecel    The time to decelerate from max velocity to final velocity.
   * @param ifinalVel The final velocity at the end of the path. If lower than min velocity, the
   *                  robot will decelerate sooner but will not go slower than the min velocity.
   * @param ik        Optional. How much to slow down around turns. The velocity of the robot is
   *                  limited by this value divided by the path curvature. A higher curvature means
   *                  a lower velocity.
   */
  PursuitLimits(const QSpeed& iminVel, const QTime& iaccel, const QSpeed& imaxVel,
                const QTime& idecel, const QSpeed& ifinalVel,
                const std::optional<QSpeed>& ik = std::nullopt);

  /**
   * Time is used for acceleration and deceleration. Deceleration and final velocity is inferred
   * from acceleration and minimum velocity.
   *
   * @param iminVel The minimum velocity through the entire path.
   * @param iaccel  The time to accelerate from min velocity to max velocity.
   * @param imaxVel The maximum velocity.
   * @param ik      Optional. How much to slow down around turns. The velocity of the robot is
   *                limited by this value divided by the path curvature. A higher curvature means a
   *                lower velocity.
   */
  PursuitLimits(const QSpeed& iminVel, const QTime& iaccel, const QSpeed& imaxVel,
                const std::optional<QSpeed>& ik = std::nullopt);

  /**
   * Shorthand. GearsetRatioPair contains a gearset (rpm) and a ratio.
   */
  using Gearset = AbstractMotor::GearsetRatioPair;

  /**
   * Scales and motor percentages are used to determine the limits. Time is used for acceleration
   * and deceleration.
   *
   * @param iwheelDiam The wheel diameter.
   * @param igearset   The wheel gearset and ratio.
   * @param imin       The minimum motor percentage through the entire path.
   * @param iaccel     The time to accelerate from min speed to max speed.
   * @param imax       The maximum motor percentage.
   * @param idecel     The time to decelerate from max speed to final speed.
   * @param ifinal     The final motor percentage at the end of the path. If lower than min speed,
   *                   the robot will decelerate sooner but will not go slower than the min speed.
   * @param ik         Optional. How much to slow down around turns. The velocity of the robot is
   *                   limited by this value divided by the path curvature. A higher curvature means
   *                   a lower velocity.
   */
  PursuitLimits(const QLength& iwheelDiam, const Gearset& igearset, double imin,
                const QTime& iaccel, double imax, const QTime& idecel, double ifinal,
                const std::optional<QSpeed>& ik = std::nullopt);

  /**
   * Scales and motor percentages are used to determine the limits. Time is used for acceleration
   * and deceleration. Deceleration and final speed is inferred from acceleration and minimum speed.
   * This is the easiest constructor to use.
   *
   * @param iwheelDiam The wheel diameter.
   * @param igearset   The wheel gearset and ratio.
   * @param imin       The minimum motor percentage through the entire path.
   * @param iaccel     The time to accelerate from min speed to max speed.
   * @param imax       The maximum motor percentage.
   * @param ik         Optional. How much to slow down around turns. The velocity of the robot is
   *                   limited by this value divided by the path curvature. A higher curvature means
   *                   a lower velocity.
   */
  PursuitLimits(const QLength& iwheelDiam, const Gearset& igearset, double imin,
                const QTime& iaccel, double imax, const std::optional<QSpeed>& ik = std::nullopt);

  /**
   * The robot's top speed and motor percentages are used to determine the limits. Time is used for
   * acceleration and deceleration.
   *
   * @param itopSpeed The robot's top velocity at 100% motor speed.
   * @param imin      The minimum motor percentage through the entire path.
   * @param iaccel    The time to accelerate from min speed to max speed.
   * @param imax      The maximum motor percentage.
   * @param idecel    The time to decelerate from max speed to final speed.
   * @param ifinal    The final motor percentage at the end of the path. If lower than min speed,
   *                  the robot will decelerate sooner but will not go slower than the min speed.
   * @param ik        Optional. How much to slow down around turns. The velocity of the robot is
   *                  limited by this value divided by the path curvature. A higher curvature means
   *                  a lower velocity.
   */
  PursuitLimits(const QSpeed& itopSpeed, double imin, const QTime& iaccel, double imax,
                const QTime& idecel, double ifinal, const std::optional<QSpeed>& ik = std::nullopt);

  /**
   * The robot's top speed and motor percentages are used to determine the limits. Time is used for
   * acceleration and deceleration. Deceleration and final speed is inferred from acceleration and
   * minimum speed.
   *
   * @param itopSpeed The robot's top velocity at 100% motor speed.
   * @param imin      The minimum motor percentage through the entire path.
   * @param iaccel    The time to accelerate from min speed to max speed.
   * @param imax      The maximum motor percentage.
   * @param ik        Optional. How much to slow down around turns. The velocity of the robot is
   *                  limited by this value divided by the path curvature. A higher curvature means
   *                  a lower velocity.
   */
  PursuitLimits(const QSpeed& itopSpeed, double imin, const QTime& iaccel, double imax,
                const std::optional<QSpeed>& ik = std::nullopt);

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
};
} // namespace lib7842
