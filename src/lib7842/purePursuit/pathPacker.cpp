#include "pathPacker.hpp"

namespace lib7842 {

PackedPath PathPacker::generate(const AbstractPath& ipath, const velGains& ivelGains) {
  ReferencePath refPath = ipath.extractRef();
  PackedPath path({refPath().begin(), refPath().end()});

  packDistances(path);
  packCurvatures(path);
  packVelocity(path, ivelGains);
  packLimitVelocity(path, ivelGains);

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

// void PathPacker::packCurvatures(PackedPath& ipath) {
//   path[0].setData("curvature", 0_curv);
//   for (size_t i = 1; i < path.size() - 1; i++) {
//     QCurvature curv = computeSingleCurvature(path[i - 1], path[i], path[i + 1]);
//     path[i].setData("curvature", curv);
//   }
//   path.back().setData("curvature", 0_curv);
//
// }

// // void PathPacker::packVelocity(PackedPath& ipath), const velGains& ivelGains) {
// //   path[path.size() - 1].setData("velocity", 0_mps);
// //   for (size_t i = path.size() - 1; i > 0; i--) {
// //     Vector& start = path[i];
// //     Vector& end = path[i - 1];
// //     QSpeed wantedVel = std::min(maxVel, (k / path[i].curvature));
// //     let distance = distPathPoint(start, end);
// //     let newVel =
// //       std::min(wantedVel, std::sqrt(std::pow(start.velocity, 2) + (2 * maxRate * distance)));
// //     path[i - 1].setData("velocity", newVel);
// //   }
// //   return path;
// // }

// // void PathPacker::packLimitVelocity(PackedPath& ipath), path, minVel, maxRate) {
// //   path[0].setData("velocity", minVel);
// //   for (let i = 0; i < path.size() - 1; i++) {
// //     let start = path[i];
// //     let end = path[i + 1];
// //     let distance = distPathPoint(start, end);
// //     let wantedVel =
// //       std::min(end.velocity, std::sqrt(std::pow(start.velocity, 2) + (2 * maxRate * distance)));
// //     let newVel = std::max(wantedVel, minVel);
// //     path[i + 1].setData("velocity", newVel);
// //   }
// //   return path;
// // }

// QCurvature PathPacker::computeSingleCurvature(
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