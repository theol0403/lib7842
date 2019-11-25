#include "pathGenerator.hpp"

namespace lib7842 {

DataPath PathGenerator::generate(const SimplePath& ipath,
                                 const QSpeed& maxVel,
                                 const QAcceleration& accel,
                                 double k) {
  DataPath path(ipath);

  setCurvatures(path);
  setMaxVelocity(path, maxVel, accel, k);

  return path;
}

void PathGenerator::setCurvatures(DataPath& ipath) {
  ipath().at(0)->setData("curvature", 0_curv);
  for (size_t i = 1; i < ipath().size() - 1; i++) {
    QCurvature curv = calculateCurvature(*ipath()[i - 1], *ipath()[i], *ipath()[i + 1]);
    ipath()[i]->setData("curvature", curv);
  }
  ipath().back()->setData("curvature", 0_curv);
}

void PathGenerator::setMaxVelocity(DataPath& ipath,
                                   const QSpeed& maxVel,
                                   const QAcceleration& accel,
                                   double k) {
  ipath().back()->setData("velocity", 0_mps);
  for (size_t i = ipath().size() - 1; i > 0; i--) {
    DataPoint& start = *ipath()[i];
    DataPoint& end = *ipath()[i - 1];

    // k / curvature, limited to max
    double wantedVel = std::min(
      maxVel.convert(mps), k / ipath()[i]->getData<QCurvature>("curvature").convert(curvature));

    // distance from last point
    double distance = Vector::dist(start, end).convert(meter);

    // maximum velocity given distance respecting acceleration
    // vf = sqrt(vi2 + 2ad)
    double maxIncrement = std::sqrt(std::pow(start.getData<QSpeed>("velocity").convert(mps), 2) +
                                    (2 * accel.convert(mps2) * distance));

    // limiting to maximum accelerated velocity
    double newVel = std::min(wantedVel, maxIncrement);
    end.setData("velocity", newVel * mps);
  }
}

QCurvature
  PathGenerator::calculateCurvature(const Vector& prev, const Vector& point, const Vector& next) {
  double distOne = Vector::dist(point, prev).convert(meter);
  double distTwo = Vector::dist(point, next).convert(meter);
  double distThree = Vector::dist(next, prev).convert(meter);

  double productOfSides = distOne * distTwo * distThree;
  double semiPerimeter = (distOne + distTwo + distThree) / 2;

  double triangleArea = std::sqrt(semiPerimeter * //
                                  (semiPerimeter - distOne) * //
                                  (semiPerimeter - distTwo) * //
                                  (semiPerimeter - distThree));

  double r = (productOfSides) / (4 * triangleArea);
  double curv = std::isnormal(1 / r) ? 1 / r : 0;
  return curv * curvature;
}

} // namespace lib7842