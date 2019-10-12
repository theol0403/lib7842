#pragma once
#include "main.h"

#include "lib7842/path/simplePath.hpp"
#include "lib7842/path/packedPath.hpp"
#include "lib7842/other/utility.hpp"

namespace lib7842 {

class PathPacker {

public:
  struct velGains {
    /**
  	 * Minumum velocity for all points
  	 */
    const QSpeed min;

    /**
  	 * Maximum velocity for all points
  	 */
    const QSpeed max;

    /**
  	 * Maximum Acceleration between points
  	 */
    const QAcceleration accel;

    /**
     * How much to slow down around turns.
     * This value is usually best around 0.5-2, 0.5 tends to slow down around almost any curvature in the
     * path, and 2 tends to slow down around only a very sharp curvature.
     */
    const double curvatureK;
  };

  static PackedPath generate(const AbstractPath& ipath, const velGains& ivelGains);

  static void packDistances(PackedPath& ipath);
  static void packCurvatures(PackedPath& ipath);

  static void packVelocity(PackedPath& ipath, const velGains& ivelGains);
  static void packLimitVelocity(PackedPath& ipath, const velGains& ivelGains);

  static QCurvature getCurvature(const Vector& prev, const Vector& point, const Vector& next);
};

} // namespace lib7842