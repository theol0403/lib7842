#pragma once
#include "lib7842/api/other/global.hpp"
#include "lib7842/api/other/utility.hpp"
#include "lib7842/api/positioning/path/discretePath.hpp"
#include "okapi/api/chassis/model/chassisModel.hpp"
#include "okapi/api/odometry/odometry.hpp"
#include "okapi/api/units/QAngularSpeed.hpp"
#include "okapi/api/units/QSpeed.hpp"
#include "pursuitLimits.hpp"
#include "pursuitPath.hpp"
#include <optional>

namespace lib7842 {

class PathFollower {
public:
  /**
   * PathFollower. Follows a generated path using the Pure Pursuit algorithm.
   *
   * @param imodel         The chassis model.
   * @param iodometry      The odometry.
   * @param ichassisScales The powered wheel scales.
   * @param igearset       The powered wheel gearset multiplied by any external gear ratio.
   * @param ilookahead     The lookahead distance.
   * @param idriveRadius   Optional. The radius from the end of the path to turn off angle
   *                       correction. Defaults to lookahead distance.
   */
  PathFollower(std::shared_ptr<ChassisModel> imodel, std::shared_ptr<Odometry> iodometry,
               const ChassisScales& ichassisScales, const QAngularSpeed& igearset,
               const QLength& ilookahead,
               const std::optional<QLength>& idriveRadius = std::nullopt);

  virtual ~PathFollower() = default;

  /**
   * Follow a pre-generated PursuitPath.
   *
   * @param ipath       The path to follow. Must have velocity setpoints generated by PathGenerator.
   * @param ibackwards  Whether to follow the path while driving backwards. The robot will follow
   *                    the exact same path as when driving forward, except it will face 180 degrees
   *                    and drive backwards.
   * @param istartSpeed Optional. The starting speed of the robot. Defaults to the min speed of the
   *                    path limits. Used to chain paths together without accelerating from zero.
   */
  void followPath(const PursuitPath& ipath, bool ibackwards = false,
                  const std::optional<QSpeed>& istartSpeed = std::nullopt);

  /**
   * Set the motor mode for the pursuit. Velocity mode is more accurate, but voltage mode is
   * smoother. Default is voltage.
   *
   * @param imode The motor mode
   */
  virtual void setMotorMode(util::motorMode imode);

protected:
  /**
   * Iterator type that points to a PursuitPath array member.
   */
  using pathIterator_t = PursuitPath::array_t::const_iterator;

  /**
   * Return an iterator to the point on a path that is closest to a position. Considers all options
   * from the last closest point to one point ahead of the lookahead. Will consider to the end of
   * the path if the end is within the lookahead.
   *
   * @param  ipath The path
   * @param  ipos  The position
   * @return iterator to the closest point
   */
  pathIterator_t findClosest(const PursuitPath& ipath, const Vector& ipos);

  /**
   * Return the lookahead point on the path. Updates lastLookIndex and lastLookT.
   *
   * @param  ipath The path
   * @param  ipos  The pos
   * @return The lookahead point
   */
  Vector findLookaheadPoint(const PursuitPath& ipath, const Vector& ipos);

  /**
   * Calculate the intersection of a lookahead circle with two points and return the interpolation
   * ratio. Return nullopt if no intersection found.
   *
   * @param  start     The starting point
   * @param  end       The ending point
   * @param  pos       The robot position
   * @param  lookahead The lookahead distance
   * @return The intersection ratio, if found
   */
  static std::optional<double> findIntersectT(const Vector& start, const Vector& end,
                                              const Vector& pos, const QLength& lookahead);

  /**
   * Calculate the curvature from the robot position and heading to the lookahead point.
   *
   * @param  state     The robot state (position and heading)
   * @param  lookPoint The lookahead point
   * @return The curvature
   */
  static double calculateCurvature(const State& state, const Vector& lookPoint);

  /**
   * Calculate the rotational velocity of each wheel given a desired robot velocity and curvature.
   *
   * @param  vel           The robot velocity
   * @param  curvature     The curvature
   * @param  chassisScales The chassis scales
   * @param  limits        The pursuit limits
   * @return The rotational velocity for each wheel.
   */
  static std::valarray<QAngularSpeed> calculateVelocity(const QSpeed& vel, double curvature,
                                                        const ChassisScales& chassisScales,
                                                        const PursuitLimits& limits);

  /**
   * Reset the pursuit members
   */
  void resetPursuit();

protected:
  std::shared_ptr<ChassisModel> model {nullptr};
  std::shared_ptr<Odometry> odometry {nullptr};
  const ChassisScales chassisScales;
  const QAngularSpeed gearset;

  const QLength lookahead {0_in};
  const QLength driveRadius {0_in};

  util::motorMode mode {util::motorMode::voltage};

  std::optional<pathIterator_t> lastClosest {std::nullopt};
  size_t lastLookIndex {0};
  double lastLookT {0};
};
} // namespace lib7842
