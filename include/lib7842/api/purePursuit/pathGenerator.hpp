#pragma once
#include "lib7842/api/positioning/point/mathPoint.hpp"
#include "pursuitLimits.hpp"
#include "pursuitPath.hpp"

namespace lib7842 {

class PathGenerator {
public:
  /**
   * Generate a PursuitPath containing waypoint information for pure pursuit.
   *
   * @param  ipath  The path
   * @param  limits The pure pursuit limits
   * @return the generated path
   */
  static PursuitPath generate(const SimplePath& ipath, const PursuitLimits& limits);

  /**
   * Generate a PursuitPath containing waypoint information for pure pursuit and heading information.
   *
   * @param  ipath  The path
   * @param  limits The pure pursuit limits
   * @return the generated path
   */
  static PursuitPath generateX(const StatePath& ipath, const PursuitLimits& limits);

protected:
  /**
   * Sets the waypoint curvatures.
   *
   * @param ipath The path
   */
  static void setCurvatures(PursuitPath& ipath);

  /**
   * Sets the waypoint velocities respecting curvature and deceleration. Traverses the path
   * backwards using a rate limiter.
   *
   * @param ipath  The path
   * @param limits The pure pursuit limits
   */
  static void setVelocity(PursuitPath& ipath, const PursuitLimits& limits);

  /**
   * Gets the curvature of a given segment.
   *
   * @param  prev  The previous point
   * @param  point The current point
   * @param  next  The next point
   * @return The curvature
   */
  static double calculateCurvature(const Vector& prev, const Vector& point, const Vector& next);
};
} // namespace lib7842
