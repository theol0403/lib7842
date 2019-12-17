#include "pathGenerator.hpp"

namespace lib7842 {

PursuitPath PathGenerator::generate(const SimplePath& ipath, const PursuitLimits& limits) {
  PursuitPath path(ipath);
  path.setLimits(limits);

  setCurvatures(path);
  setMaxVelocity(path, limits);

  return path;
}

void PathGenerator::setCurvatures(PursuitPath& ipath) {
  ipath().at(0)->setData("curvature", 0.0);
  for (size_t i = 1; i < ipath().size() - 1; i++) {
    double curvature = calculateCurvature(*ipath()[i - 1], *ipath()[i], *ipath()[i + 1]);
    ipath()[i]->setData("curvature", curvature);
  }
  ipath().back()->setData("curvature", 0.0);
}

void PathGenerator::setMaxVelocity(PursuitPath& ipath, const PursuitLimits& limits) {
  ipath().back()->setData("velocity", limits.finalVel);
  for (size_t i = ipath().size() - 1; i > 0; i--) {
    DataPoint& start = *ipath()[i];
    DataPoint& end = *ipath()[i - 1];

    // k / curvature, limited to max
    QSpeed wantedVel = std::min(limits.maxVel, limits.k / ipath()[i]->getData<double>("curvature"));

    // distance from last point
    double distance = MathPoint::dist(start, end);

    // maximum velocity given distance respecting acceleration
    // vf = sqrt(vi2 + 2ad)
    QSpeed maxIncrement =
      mps * std::sqrt(std::pow(start.getData<QSpeed>("velocity").convert(mps), 2) +
                      (2.0 * limits.decel.convert(mps2) * distance));

    // limiting to maximum accelerated velocity
    QSpeed newVel = std::min(wantedVel, maxIncrement);
    end.setData("velocity", newVel);
  }
}

double PathGenerator::calculateCurvature(const Vector& prev, const Vector& point,
                                         const Vector& next) {
  double distOne = MathPoint::dist(point, prev);
  double distTwo = MathPoint::dist(point, next);
  double distThree = MathPoint::dist(next, prev);

  double productOfSides = distOne * distTwo * distThree;
  double semiPerimeter = (distOne + distTwo + distThree) / 2.0;

  double triangleArea = std::sqrt(semiPerimeter * //
                                  (semiPerimeter - distOne) * //
                                  (semiPerimeter - distTwo) * //
                                  (semiPerimeter - distThree));

  double r = productOfSides / (4.0 * triangleArea);
  double curvature = std::isnormal(1.0 / r) ? 1.0 / r : 0;
  return curvature * curvature;
}

} // namespace lib7842