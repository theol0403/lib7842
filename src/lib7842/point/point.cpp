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
 * QPoint Accessors
 */
QLength& QPoint::at(const size_t& iindex) {
  switch (iindex) {
    case 0: return x; break;
    case 1: return y; break;
    default:
      throw std::runtime_error(
        "QPoint::at():: \"" + std::to_string(iindex) + "\" is invalid index");
      break;
  }
}

QLength& QPoint::operator[](const size_t& iindex) {
  return at(iindex);
}

const QLength& QPoint::get(const size_t& iindex) const {
  return const_cast<QPoint*>(this)->at(iindex);
}

/**
 * QPoint Vector Functoins
 */
QPoint QPoint::operator*(const double scalar) const {
  return {x * scalar, y * scalar};
}

QPoint QPoint::operator/(const double scalar) const {
  return {x / scalar, y / scalar};
}

QPoint QPoint::normalize(const QPoint& lhs) {
  return lhs / mag(lhs).convert(meter);
}

QPoint QPoint::scalarMult(const QPoint& lhs, const double scalar) {
  return lhs * scalar;
}

QArea QPoint::dot(const QPoint& lhs, const QPoint& rhs) {
  return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}

QLength QPoint::mag(const QPoint& lhs) {
  return meter * std::sqrt((lhs.x * lhs.x + lhs.y * lhs.y).convert(meter2));
}

QLength QPoint::dist(const QPoint& lhs, const QPoint& rhs) {
  return meter
         * std::sqrt(
           ((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y)).convert(meter2));
}

} // namespace lib7842