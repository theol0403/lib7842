#pragma once
#include "main.h"
#include "pathPoint.hpp"
#include "pathSegment.hpp"
#include "lib7842/other/utility.hpp"

namespace lib7842 {

class PathGenerator {

  struct smoothGains {
    const double& weight;
    const double& threshold;
  };

  struct velGains {
    const QSpeed& min;
    const QSpeed& max;
    const QAcceleration& accel;
  };

  using Path = std::vector<PathPoint>;

 public:
  PathGenerator(const PathSegment& isegment);

  Path generate(
    const QLength& iresolution, const smoothGains& ismoothGains, const velGains& ivelGains);

  PathGenerator& insertPoints(const QLength& iresolution);
  PathGenerator& smoothen(const smoothGains& ismoothGains);

  PathGenerator& computeDistances();
  PathGenerator& computeCurvatures();

  PathGenerator& computeVelocity(const velGains& ivelGains);
  PathGenerator& limitVelocity(const velGains& ivelGains);

  Path get() const;
  Path& getRef();

  static double
    computeSingleCurvature(const QPoint& prevPoint, const QPoint& point, const QPoint& nextPoint);

 protected:
  Path path;
};

} // namespace lib7842