#include "point.hpp"

namespace lib7842 {

/**
 * QPoint Constructor
 */
QPoint::QPoint(const QLength& ix, const QLength& iy) : x(ix), y(iy) {}

/**
 * QPoint Math Operators
 */
QPoint QPoint::operator+(const QPoint& rhs) const {
  return {x + rhs.x, y + rhs.y};
}

QPoint QPoint::operator-(const QPoint& rhs) const {
  return {x - rhs.x, y - rhs.y};
}

/**
 * QPoint Equality Operators
 */
bool QPoint::operator==(const QPoint& rhs) const {
  return x == rhs.x && y == rhs.y;
}

bool QPoint::operator!=(const QPoint& rhs) const {
  return !(rhs == *this);
}

/**
 * QPoint Accessor Operator
 */
QLength& QPoint::operator[](const size_t& iindex) {
  switch (iindex) {
    case 0: return x; break;
    case 1: return y; break;
    default:
      throw std::runtime_error(
        "QPoint::operator[]:: \"" + std::to_string(iindex) + "\" is invalid index");
      break;
  }
}

/**
 * QPoint Vector Functoins
 */
QPoint QPoint::normalize() const {
  return {x / mag().convert(meter), y / mag().convert(meter)};
}

QPoint QPoint::scalarMult(const double& scalar) const {
  return {x * scalar, y * scalar};
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

} // namespace lib7842