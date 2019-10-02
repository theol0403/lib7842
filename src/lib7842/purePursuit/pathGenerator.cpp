#include "pathGenerator.hpp"

namespace lib7842 {

PathGenerator::PathGenerator(const PathSegment& isegment) : path(isegment.extract()) {}

// PathGenerator::Path PathGenerator::generate(
//   const QLength& iresolution, const smoothGains& ismoothGains, const velGains& ivelGains) {
//   return (*this)
//     .insertPoints(iresolution)
//     .smoothen(ismoothGains)
//     .computeDistances()
//     .computeCurvatures()
//     .computeVelocity(ivelGains)
//     .limitVelocity(ivelGains)
//     .get();
// }

PathGenerator& PathGenerator::insertPoints(const QLength& iresolution) {
  Path newPath;

  for (size_t i = 0; i < path.size() - 1; i++) {
    QPoint& start = path[i];
    QPoint& end = path[i + 1];
    QPoint diff = end - start;

    size_t numInsert =
      std::ceil((diff.mag() / iresolution).convert(number)); // number of points needed

    newPath.reserve(newPath.capacity() + numInsert); // reserve vector capacity

    QPoint step = diff.scalarMult(1.0 / numInsert); // how much to increment each point

    for (size_t j = 0; j < numInsert; j++) {
      QLength xNew = start.x + step.x * j;
      QLength yNew = start.y + step.y * j;
      PathPoint newPoint(xNew, yNew);
      newPoint.setData("segmentIndex", i);
      newPath.push_back(std::move(newPoint));
    }
  }

  // push the last point
  if (path.size() > 0) newPath.push_back(std::move(path.back()));
  path = std::move(newPath);
  return *this;
}

// PathGenerator& PathGenerator::smoothen(inp, dataWeight, tolerance) {
//   let path = _.cloneDeep(inp); //copy
//   let smoothWeight = 1.0 - dataWeight;
//   let change = tolerance;
//   while (change >= tolerance) {
//     change = 0.0;
//     for (let i = 1; i < inp.size() - 1; i++) {
//       for (let j = 0; j < 2; j++) {
//         let aux = path[i].loc[j];
//         let dataFac = dataWeight * (inp[i].loc[j] - path[i].loc[j]);
//         let smoothFac =
//           smoothWeight * (path[i - 1].loc[j] + path[i + 1].loc[j] - (2.0 * path[i].loc[j]));
//         path[i].loc[j] += (dataFac + smoothFac);
//         change = std::abs(aux - path[i].loc[j]);
//       }
//     }
//   }
//   return path;
// }

// PathGenerator& PathGenerator::computeDistances(path) {
//   path[0].setDistance(0);
//   for (let i = 0; i < path.size() - 1; i++) {
//     let distance = path[i].distance + distPathPoint(path[i], path[i + 1]);
//     path[i + 1].setDistance(distance);
//   }
//   return path;
// }

// PathGenerator& PathGenerator::computeSingleCurvature(prevPoint, point, nextPoint) {
//   let distOne = distPathPoint(point, prevPoint);
//   let distTwo = distPathPoint(point, nextPoint);
//   let distThree = distPathPoint(nextPoint, prevPoint);

//   let productOfSides = distOne * distTwo * distThree;
//   let semiPerimeter = (distOne + distTwo + distThree) / 2;
//   let triangleArea = std::sqrt(
//     semiPerimeter * (semiPerimeter - distOne) * (semiPerimeter - distTwo)
//     * (semiPerimeter - distThree));

//   let r = (productOfSides) / (4 * triangleArea);
//   let curvature = isNaN(1 / r) ? 0 : 1 / r;
//   return curvature;
// }

// PathGenerator& PathGenerator::computeCurvatures(path) {
//   path[0].setCurvature(0);
//   for (let i = 1; i < path.size() - 1; i++) {
//     let curvature = computeSingleCurvature(path[i - 1], path[i], path[i + 1]);
//     path[i].setCurvature(curvature);
//   }
//   path[path.size() - 1].setCurvature(0);
//   return path;
// }

// PathGenerator& PathGenerator::computeVelocity(path, maxVel, maxRate, k) {
//   path[path.size() - 1].setVelocity(0);
//   for (let i = path.size() - 1; i > 0; i--) {
//     let start = path[i];
//     let end = path[i - 1];
//     let wantedVel = std::min(maxVel, (k / path[i].curvature));
//     let distance = distPathPoint(start, end);
//     let newVel =
//       std::min(wantedVel, std::sqrt(std::pow(start.velocity, 2) + (2 * maxRate * distance)));
//     path[i - 1].setVelocity(newVel);
//   }
//   return path;
// }

// PathGenerator& PathGenerator::limitVelocity(path, minVel, maxRate) {
//   path[0].setVelocity(minVel);
//   for (let i = 0; i < path.size() - 1; i++) {
//     let start = path[i];
//     let end = path[i + 1];
//     let distance = distPathPoint(start, end);
//     let wantedVel =
//       std::min(end.velocity, std::sqrt(std::pow(start.velocity, 2) + (2 * maxRate * distance)));
//     let newVel = std::max(wantedVel, minVel);
//     path[i + 1].setVelocity(newVel);
//   }
//   return path;
// }
} // namespace lib7842