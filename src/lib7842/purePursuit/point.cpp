#include "point.hpp"

namespace lib7842 {

/**
  * QPoint
  */
QPoint::QPoint(const QPoint& ipoint) : x(ipoint.x), y(ipoint.y), theta(ipoint.theta) {}

QPoint::QPoint(QLength ix, QLength iy, QAngle itheta) : x(ix), y(iy), theta(itheta) {}
QPoint::QPoint(QLength ix, QLength iy) : x(ix), y(iy) {}
QPoint::QPoint() {}

QPoint::QPoint(dPoint ipoint) : x(ipoint.x * inch), y(ipoint.y * inch), theta(ipoint.theta * radian) {}

QPoint QPoint::operator+(QPoint rhs) {
  return {x + rhs.x, y + rhs.y, theta + rhs.theta};
}

/**
  * dPoint
  */
dPoint::dPoint(const dPoint& ipoint) : x(ipoint.x), y(ipoint.y), theta(ipoint.theta) {}

dPoint::dPoint(double ix, double iy, double itheta) : x(ix), y(iy), theta(itheta) {}
dPoint::dPoint(double ix, double iy) : x(ix), y(iy) {}
dPoint::dPoint() {}

dPoint::dPoint(QPoint ipoint) : x(ipoint.x.convert(inch)), y(ipoint.y.convert(inch)), theta(ipoint.theta.convert(radian)) {}

dPoint dPoint::operator+(dPoint rhs) {
  return {x + rhs.x, y + rhs.y, theta + rhs.theta};
}

} // namespace lib7842