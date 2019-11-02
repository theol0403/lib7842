#include "pathGenerator.hpp"

namespace lib7842 {

PathGenerator::limits::limits(QSpeed imin, QSpeed imax, QAcceleration iaccel, QCurvature icurvatureK) :
  min(imin.convert(mps)),
  max(imax.convert(mps)),
  accel(iaccel.convert(mps2)),
  curvatureK(icurvatureK.convert(curvature)) {}

DataPath PathGenerator::generate(const SimplePath& ipath, const limits& ilimits) {
  DataPath path(ipath);

  setDistances(path);
  setCurvatures(path);
  setMaxVelocity(path, ilimits);
  setMinVelocity(path, ilimits);

  return path;
}

void PathGenerator::setDistances(DataPath& ipath) {
  ipath().at(0)->setData("distance", 0_in);
  for (size_t i = 0; i < ipath().size() - 1; i++) {
    QLength distance =
      ipath()[i]->getData<QLength>("distance") + Vector::dist(*ipath()[i], *ipath()[i + 1]);
    ipath()[i + 1]->setData("distance", distance);
  }
}

void PathGenerator::setCurvatures(DataPath& ipath) {
  ipath().at(0)->setData("curvature", 0_curv);
  for (size_t i = 1; i < ipath().size() - 1; i++) {
    QCurvature curv = getCurvature(*ipath()[i - 1], *ipath()[i], *ipath()[i + 1]);
    ipath()[i]->setData("curvature", curv);
  }
  ipath().back()->setData("curvature", 0_curv);
}

void PathGenerator::setMaxVelocity(DataPath& ipath, const limits& ilimits) {
  ipath().back()->setData("velocity", 0_mps);
  for (size_t i = ipath().size() - 1; i > 0; i--) {
    DataPoint& start = *ipath()[i];
    DataPoint& end = *ipath()[i - 1];

    // k / curvature, limited to max
    double wantedVel = std::min(
      ilimits.max, ilimits.curvatureK / ipath()[i]->getData<QCurvature>("curvature").convert(curvature));

    // distance from last point
    double distance = Vector::dist(start, end).convert(meter);

    // maximum velocity given distance respecting acceleration
    // vf = sqrt(vi2 + 2ad)
    double maxIncrement = std::sqrt(
      std::pow(start.getData<QSpeed>("velocity").convert(mps), 2) + (2 * ilimits.accel * distance));

    // limiting to maximum accelerated velocity
    double newVel = std::min(wantedVel, maxIncrement);
    end.setData("velocity", newVel * mps);
  }
}

void PathGenerator::setMinVelocity(DataPath& ipath, const limits& ilimits) {
  ipath().at(0)->setData("velocity", ilimits.min * mps);
  for (size_t i = 0; i < ipath().size() - 1; i++) {
    DataPoint& start = *ipath()[i];
    DataPoint& end = *ipath()[i + 1];

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
    end.setData("velocity", newVel * mps);
  }
}

QCurvature PathGenerator::getCurvature(const Vector& prev, const Vector& point, const Vector& next) {
  double distOne = Vector::dist(point, prev).convert(meter);
  double distTwo = Vector::dist(point, next).convert(meter);
  double distThree = Vector::dist(next, prev).convert(meter);

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