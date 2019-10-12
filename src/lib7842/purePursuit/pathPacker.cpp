#include "pathPacker.hpp"

namespace lib7842 {

PackedPath PathPacker::generate(const AbstractPath& ipath, const limits& ilimits) {
  ReferencePath refPath = ipath.extractRef();
  PackedPath path({refPath().begin(), refPath().end()});

  packDistances(path);
  packCurvatures(path);
  packVelocity(path, ilimits);
  packLimitVelocity(path, ilimits);

  return path;
}

void PathPacker::packDistances(PackedPath& ipath) {
  ipath().front().setData("distance", 0_in);
  for (size_t i = 0; i < ipath().size() - 1; i++) {
    QLength distance =
      ipath()[i].getData<QLength>("distance") + Vector::dist(ipath()[i], ipath()[i + 1]);
    ipath()[i + 1].setData("distance", distance);
  }
}

void PathPacker::packCurvatures(PackedPath& ipath) {
  ipath().front().setData("curvature", 0_curv);
  for (size_t i = 1; i < ipath().size() - 1; i++) {
    QCurvature curv = getCurvature(ipath()[i - 1], ipath()[i], ipath()[i + 1]);
    ipath()[i].setData("curvature", curv);
  }
  ipath().back().setData("curvature", 0_curv);
}

void PathPacker::packVelocity(PackedPath& ipath, const limits& ilimits) {
  ipath().back().setData("velocity", 0_mps);
  for (size_t i = ipath().size() - 1; i > 0; i--) {
    PackedPoint& start = ipath()[i];
    PackedPoint& end = ipath()[i - 1];

    // k / curvature, limited to max
    double wantedVel = std::min(
      ilimits.max,
      ilimits.curvatureK / ipath()[i].getData<QCurvature>("curvature").convert(curvature));

    // distance from last point
    double distance = Vector::dist(start, end).convert(meter);

    // maximum velocity given distance respecting acceleration
    // vf = sqrt(vi2 + 2ad)
    double maxIncrement = std::sqrt(
      std::pow(start.getData<QSpeed>("velocity").convert(mps), 2) + (2 * ilimits.accel * distance));

    // limiting to maximum accelerated velocity
    double newVel = std::min(wantedVel, maxIncrement);

    ipath()[i - 1].setData("velocity", newVel * mps);
  }
}

void PathPacker::setMinVelocity(PackedPath& ipath, const limits& ilimits) {
  ipath().front().setData("velocity", ilimits.min * mps);
  for (size_t i = 0; i < ipath().size() - 1; i++) {
    PackedPoint& start = ipath()[i];
    PackedPoint& end = ipath()[i + 1];

    // distance to next point
    double distance = Vector::dist(start, end).convert(meter);

    // maximum velocity given distance respecting acceleration
    // vf = sqrt(vi2 + 2ad)
    double maxIncrement = std::sqrt(
      std::pow(start.getData<QSpeed>("velocity").convert(mps), 2) + (2 * ilimits.accel * distance));

    // limiting to maximum accelerated velocity
    double wantedVel = std::min(end.getData<QSpeed>("velocity").convert(mps), maxIncrement);

    // limiting to minimum vel
    double newVel = std::max(wantedVel, ilimits.min);
    ipath()[i + 1].setData("velocity", newVel * mps);
  }
}

// QCurvature PathPacker::getCurvature(
//   const Vector& prevPoint, const Vector& point, const Vector& nextPoint) {
//   double distOne = point.dist(prevPoint).convert(meter);
//   double distTwo = point.dist(nextPoint).convert(meter);
//   double distThree = nextPoint.dist(prevPoint).convert(meter);

//   double productOfSides = distOne * distTwo * distThree;
//   double semiPerimeter = (distOne + distTwo + distThree) / 2;

//   double triangleArea = std::sqrt(
//     semiPerimeter * //
//     (semiPerimeter - distOne) * //
//     (semiPerimeter - distTwo) * //
//     (semiPerimeter - distThree));

//   double r = (productOfSides) / (4 * triangleArea);
//   double curv = std::isnormal(1 / r) ? 1 / r : 0;
//   return curv * curvature;
// }

} // namespace lib7842