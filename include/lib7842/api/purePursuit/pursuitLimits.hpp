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
 *
 * There are four different kinds of constructors. Each kind has two versions, one with all 6 limits
 * and one with only 4 limits. K is optional in each constructor. The kinds are as follows:
 *  - Real-world units.
 *  - Real-world units but acceleration and deceleration are specified by time.
 *  - Motor percentages, where wheel dimensions and gearing are used for the calculation.
 *  - Motor percentages, where robot top speed is used for the calculation.
 *
 * The easiest constructor to use is the 4-parameter version of the motor percentages with wheel
 * dimensions and gearing.
 */
struct PursuitLimits {
  /**
   * Real-world units.
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
   * Real-world units. Deceleration and final velocity is inferred from acceleration and minimum velocity.
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
   * Real-world units. Time is used for acceleration and deceleration.
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
   * Real-world units. Time is used for acceleration. Deceleration and final velocity is inferred
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
   * Motor percentages. Wheel dimensions and gearing are used to determine the limits. Time is used
   * for acceleration and deceleration. Motor percentages are in the range (0, 1).
   *
   * @param iwheelDiam The wheel diameter.
   * @param igearset   The wheel gearset (top speed in RPM) multiplied by any external gear ratio.
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
  PursuitLimits(const QLength& iwheelDiam, double igearset, double imin, const QTime& iaccel,
                double imax, const QTime& idecel, double ifinal,
                const std::optional<QSpeed>& ik = std::nullopt);

  /**
   * Motor percentages. Wheel dimensions and gearing are used to determine the limits. Time is used
   * for acceleration and deceleration. Deceleration and final speed is inferred from acceleration
   * and minimum speed. Motor percentages are in the range (0, 1). This is the easiest constructor
   * to use.
   *
   * @param iwheelDiam The wheel diameter.
   * @param igearset   The wheel gearset (top speed in RPM) multiplied by any external gear ratio.
   * @param imin       The minimum motor percentage through the entire path.
   * @param iaccel     The time to accelerate from min speed to max speed.
   * @param imax       The maximum motor percentage.
   * @param ik         Optional. How much to slow down around turns. The velocity of the robot is
   *                   limited by this value divided by the path curvature. A higher curvature means
   *                   a lower velocity.
   */
  PursuitLimits(const QLength& iwheelDiam, double igearset, double imin, const QTime& iaccel,
                double imax, const std::optional<QSpeed>& ik = std::nullopt);

  /**
   * Motor percentages. The robot's top speed is used to determine the limits. Time is used for
   * acceleration and deceleration. Motor percentages are in the range (0, 1).
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
   * Motor percentages. The robot's top speed is used to determine the limits. Time is used for
   * acceleration and deceleration. Deceleration and final speed is inferred from acceleration and
   * minimum speed. Motor percentages are in the range (0, 1).
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
