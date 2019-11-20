#pragma once

#include "lib7842/api/other/utility.hpp"
#include "lib7842/api/positioning/path/dataPath.hpp"
#include "lib7842/api/positioning/path/simplePath.hpp"

namespace lib7842 {

class PathGenerator {
public:
  /**
   * Generate a DataPath containing waypoint information for pure pursuit.
   *
   * @param  ipath      The path
   * @param  minVel     The minimum velocity
   * @param  maxVel     The maximum velocity
   * @param  accel      The maximum acceleration
   * @param  curvatureK How much to slow down around turns. This value is usually best around 0.5-2,
   *                    0.5 tends to slow down around almost any curvature in the path, and 2 tends
   *                    to slow down around only a very sharp curvature.
   * @return the generated path
   */
  static DataPath
    generate(const SimplePath& ipath, double minVel, double maxVel, double accel, double curvatureK);

  /**
   * Sets the waypoint curvatures.
   *
   * @param ipath The path
   */
  static void setCurvatures(DataPath& ipath);

  /**
   * Sets the waypoint velocities respecting curvature and deceleration. Traverses the path
   * backwards using a rate limiter.
   *
   * @param ipath      The path
   * @param maxVel     The maximum velocity
   * @param accel      The accel
   * @param curvatureK How much to slow down around turns. This value is usually best around 0.5-2,
   *                   0.5 tends to slow down around almost any curvature in the path, and 2 tends
   *                   to slow down around only a very sharp curvature.
   */
  static void setMaxVelocity(DataPath& ipath, double maxVel, double accel, double curvatureK);

  /**
   * Sets the waypoint minimum velocities respecting acceleration. Traverses the path forward using
   * a rate limiter.
   *
   * @param ipath  The path
   * @param minVel The minimum velocity
   * @param accel  The accel
   */
  static void setMinVelocity(DataPath& ipath, double minVel, double accel);

  /**
   * Gets the curvature of a given segment.
   *
   * @param  prev  The previous point
   * @param  point The current point
   * @param  next  The next point
   * @return The curvature
   */
  static QCurvature getCurvature(const Vector& prev, const Vector& point, const Vector& next);
};

} // namespace lib7842