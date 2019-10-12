#pragma once
#include "main.h"

#include "lib7842/path/simplePath.hpp"
#include "lib7842/path/packedPath.hpp"
#include "lib7842/other/utility.hpp"

namespace lib7842 {

class PathPacker {

public:
  struct limits {
    /**
  	 * Minumum velocity for all points
  	 */
    const double min;

    /**
  	 * Maximum velocity for all points
  	 */
    const double max;

    /**
  	 * Maximum Acceleration between points
  	 */
    const double accel;

    /**
     * How much to slow down around turns.
     * This value is usually best around 0.5-2, 0.5 tends to slow down around almost any curvature in the
     * path, and 2 tends to slow down around only a very sharp curvature.
     */
    const double curvatureK;

    limits(QSpeed imin, QSpeed imax, QAcceleration iaccel, QCurvature icurvatureK);
  };

  static PackedPath generate(const AbstractPath& ipath, const limits& ilimits);

  static void setDistances(PackedPath& ipath);
  static void setCurvatures(PackedPath& ipath);

  static void setMaxVelocity(PackedPath& ipath, const limits& ilimits);
  static void setMinVelocity(PackedPath& ipath, const limits& ilimits);

  static QCurvature getCurvature(const Vector& prev, const Vector& point, const Vector& next);
};

} // namespace lib7842