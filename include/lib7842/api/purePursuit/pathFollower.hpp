#pragma once
#include "lib7842/api/other/utility.hpp"
#include "lib7842/api/positioning/path/discretePath.hpp"
#include "okapi/api/chassis/model/chassisModel.hpp"
#include "okapi/api/odometry/odometry.hpp"
#include "okapi/api/units/QAngularSpeed.hpp"
#include "okapi/api/units/QSpeed.hpp"
#include "okapi/api/util/logging.hpp"
#include "okapi/api/util/timeUtil.hpp"
#include "pursuitLimits.hpp"
#include "pursuitPath.hpp"
#include <optional>

namespace lib7842 {

class PathFollower {
public:
  /**
   * PathFollower
   *
   * @param imodel         The chassis model
   * @param iodometry      The odometry
   * @param ichassisScales The chassis scales
   * @param ilookahead     The lookahead distance
   * @param itimeUtil      The time utility
   */
  PathFollower(const std::shared_ptr<ChassisModel>& imodel,
               const std::shared_ptr<Odometry>& iodometry, const ChassisScales& ichassisScales,
               const QLength& ilookahead, const TimeUtil& itimeUtil);

  /**
   * Follow a pre-generated PursuitPath.
   *
   * @param ipath The path
   */
  void followPath(const PursuitPath& ipath);

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
  ChassisScales chassisScales;

  const QLength lookahead {0_in};
  TimeUtil timeUtil;

  std::optional<pathIterator_t> lastClosest {std::nullopt};
  size_t lastLookIndex {0};
  double lastLookT {0};
};
} // namespace lib7842
