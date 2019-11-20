#pragma once

#include "lib7842/api/other/utility.hpp"
#include "lib7842/api/positioning/path/dataPath.hpp"
#include "lib7842/api/positioning/path/simplePath.hpp"

namespace lib7842 {

class PathGenerator {
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
     * How much to slow down around turns. This value is usually best around 0.5-2, 0.5 tends to
     * slow down around almost any curvature in the path, and 2 tends to slow down around only a
     * very sharp curvature.
     */
    const double curvatureK;

    limits(QSpeed imin, QSpeed imax, QAcceleration iaccel, QCurvature icurvatureK);
  };

  static DataPath generate(const SimplePath& ipath, const limits& ilimits);

  static void setDistances(DataPath& ipath);
  static void setCurvatures(DataPath& ipath);

  static void setMaxVelocity(DataPath& ipath, const limits& ilimits);
  static void setMinVelocity(DataPath& ipath, const limits& ilimits);

  static QCurvature getCurvature(const Vector& prev, const Vector& point, const Vector& next);
};

} // namespace lib7842