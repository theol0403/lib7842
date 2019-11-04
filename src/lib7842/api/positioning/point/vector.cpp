#include "vector.hpp"

#include <stdexcept>
#include <string>

namespace lib7842 {

/**
 * Vector Constructor
 */
Vector::Vector(const QLength& ix, const QLength& iy) : x(ix), y(iy) {}

/**
 * Vector Math Operators
 */
Vector Vector::operator+(const Vector& rhs) const {
  return {x + rhs.x, y + rhs.y};
}

Vector Vector::operator-(const Vector& rhs) const {
  return {x - rhs.x, y - rhs.y};
}

/**
 * Vector Equality Operators
 */
bool Vector::operator==(const Vector& rhs) const {
  return x == rhs.x && y == rhs.y;
}

bool Vector::operator!=(const Vector& rhs) const {
  return !(rhs == *this);
}

/**
 * Vector Accessors
 */
QLength& Vector::at(const size_t& iindex) {
  switch (iindex) {
    case 0: return x; break;
    case 1: return y; break;
    default:
      throw std::runtime_error("Vector::at():: \"" + std::to_string(iindex) + "\" is invalid index");
      break;
  }
}

QLength& Vector::operator[](const size_t& iindex) {
  return at(iindex);
}

const QLength& Vector::read(const size_t& iindex) const {
  return const_cast<Vector*>(this)->at(iindex);
}

/**
 * Vector Vector Functoins
 */
Vector Vector::operator*(const double scalar) const {
  return {x * scalar, y * scalar};
}

Vector Vector::operator/(const double scalar) const {
  return {x / scalar, y / scalar};
}

Vector Vector::normalize(const Vector& lhs) {
  double imag = mag(lhs).convert(meter);
  return imag ? lhs / imag : lhs;
}

Vector Vector::scalarMult(const Vector& lhs, const double scalar) {
  return lhs * scalar;
}

QArea Vector::dot(const Vector& lhs, const Vector& rhs) {
  return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}

QLength Vector::mag(const Vector& lhs) {
  return meter * std::sqrt((lhs.x * lhs.x + lhs.y * lhs.y).convert(meter2));
}

QLength Vector::dist(const Vector& lhs, const Vector& rhs) {
  return meter *
         std::sqrt(
           ((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y)).convert(meter2));
}

} // namespace lib7842