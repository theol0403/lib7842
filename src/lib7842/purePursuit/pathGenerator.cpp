#include "pathGenerator.hpp"

namespace lib7842 {

PathGenerator::PathGenerator(const PathSegment& isegment) {
  PathSegment::PointRefList temp = isegment.extractRef();
  path = std::move(std::vector<PathPoint>(temp.begin(), temp.end()));
}

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

PathGenerator& PathGenerator::smoothen(const smoothGains& ismoothGains) {
  Path newPath = path; //copy
  double smoothWeight = 1.0 - ismoothGains.weight;
  QLength change = ismoothGains.tolerance;
  while (change >= ismoothGains.tolerance) {
    change = 0.0_in;
    for (size_t i = 1; i < path.size() - 1; i++) {
      for (size_t j = 0; j < 2; j++) {
        QLength aux = newPath[i][j];
        QLength dataFac = ismoothGains.weight * (path[i][j] - newPath[i][j]);
        QLength smoothFac =
          smoothWeight * (newPath[i - 1][j] + path[i + 1][j] - (2.0 * newPath[i][j]));
        newPath[i][j] += (dataFac + smoothFac);
        change = (aux - newPath[i][j]).abs();
      }
    }
  }
  path = std::move(newPath);
  return *this;
}

PathGenerator& PathGenerator::computeDistances() {
  path[0].setData("distance", 0_in);
  for (size_t i = 0; i < path.size() - 1; i++) {
    QLength distance = path[i].getValue<QLength>("distance") + path[i].dist(path[i + 1]);
    path[i + 1].setData("distance", std::move(distance));
  }
  return *this;
}

PathGenerator& PathGenerator::computeCurvatures() {
  path[0].setData("curvature", 0_curv);
  for (size_t i = 1; i < path.size() - 1; i++) {
    QCurvature curv = computeSingleCurvature(path[i - 1], path[i], path[i + 1]);
    path[i].setData("curvature", curv);
  }
  path.back().setData("curvature", 0_curv);
  return *this;
}

// PathGenerator& PathGenerator::computeVelocity(const velGains& ivelGains) {
//   path[path.size() - 1].setData("velocity", 0_mps);
//   for (size_t i = path.size() - 1; i > 0; i--) {
//     QPoint& start = path[i];
//     QPoint& end = path[i - 1];
//     QSpeed wantedVel = std::min(maxVel, (k / path[i].curvature));
//     let distance = distPathPoint(start, end);
//     let newVel =
//       std::min(wantedVel, std::sqrt(std::pow(start.velocity, 2) + (2 * maxRate * distance)));
//     path[i - 1].setData("velocity", newVel);
//   }
//   return path;
// }

// PathGenerator& PathGenerator::limitVelocity(path, minVel, maxRate) {
//   path[0].setData("velocity", minVel);
//   for (let i = 0; i < path.size() - 1; i++) {
//     let start = path[i];
//     let end = path[i + 1];
//     let distance = distPathPoint(start, end);
//     let wantedVel =
//       std::min(end.velocity, std::sqrt(std::pow(start.velocity, 2) + (2 * maxRate * distance)));
//     let newVel = std::max(wantedVel, minVel);
//     path[i + 1].setData("velocity", newVel);
//   }
//   return path;
// }

QCurvature PathGenerator::computeSingleCurvature(
  const QPoint& prevPoint, const QPoint& point, const QPoint& nextPoint) {
  double distOne = point.dist(prevPoint).convert(meter);
  double distTwo = point.dist(nextPoint).convert(meter);
  double distThree = nextPoint.dist(prevPoint).convert(meter);

  double productOfSides = distOne * distTwo * distThree;
  double semiPerimeter = (distOne + distTwo + distThree) / 2;

  double triangleArea = std::sqrt(
    semiPerimeter * //
    (semiPerimeter - distOne) * //
    (semiPerimeter - distTwo) * //
    (semiPerimeter - distThree));

  double r = (productOfSides) / (4 * triangleArea);
  double curv = std::isnormal(1 / r) ? 1 / r : 0;
  return curv * curvature;
}

} // namespace lib7842