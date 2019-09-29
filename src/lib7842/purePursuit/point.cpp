#include "point.hpp"

namespace lib7842 {

QPoint::QPoint(const QLength& ix, const QLength& iy) : x(ix), y(iy) {}

QPoint QPoint::operator+(const QPoint& rhs) const {
  return {x + rhs.x, y + rhs.y};
}

QPoint QPoint::operator-(const QPoint& rhs) const {
  return {x - rhs.x, y - rhs.y};
}

bool QPoint::operator==(const QPoint& rhs) const {
  return x == rhs.x && y == rhs.y;
}

QPoint QPoint::normalize() const {
  return {x / mag().convert(meter), y / mag().convert(meter)};
}

QPoint QPoint::scalarMult(const QLength& scalar) const {
  return {x * scalar.convert(meter), y * scalar.convert(meter)};
}

QArea QPoint::dot(const QPoint& rhs) const {
  return (x * rhs.x) + (y * rhs.y);
}

QLength QPoint::mag() const {
  return meter * std::sqrt((x * x + y * y).convert(meter2));
}

QLength QPoint::dist(const QPoint& rhs) const {
  return meter * std::sqrt(((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y)).convert(meter2));
}

QState::QState(const QLength& ix, const QLength& iy, const QAngle& itheta) :
  QPoint(ix, iy), theta(itheta) {}

QState::QState(const QPoint& ipoint) : QPoint(ipoint) {};

QState QState::operator+(const QState& rhs) const {
  return {x + rhs.x, y + rhs.y};
}

QState QState::operator-(const QState& rhs) const {
  return {x - rhs.x, y - rhs.y};
}

bool QState::operator==(const QState& rhs) const {
  return x == rhs.x && y == rhs.y && theta == rhs.theta;
}

} // namespace lib7842