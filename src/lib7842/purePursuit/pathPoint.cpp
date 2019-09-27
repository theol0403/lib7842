#include "pathPoint.hpp"

namespace lib7842 {

PathPoint::PathPoint() {}

PathPoint::PathPoint(QPoint ipoint) : point(ipoint) {}

QLength PathPoint::x() {
  return point.x;
}
QLength PathPoint::y() {
  return point.y;
}
QPoint PathPoint::operator()() {
  return point;
}

// void PathPoint::setDistance(QLength idistance);

// void PathPoint::setVelocity(QSpeed ivelocity);
// void PathPoint::setCurvature(double icurvature);

} // namespace lib7842