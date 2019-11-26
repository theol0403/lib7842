#include "vector.hpp"

#include <stdexcept>
#include <string>

namespace lib7842 {

/**
 * Vector Constructor
 */
Vector::Vector(const QLength& ix, const QLength& iy) : x(ix), y(iy) {}

/**
 * Vector Constructor 
 * Upcast Point 
 */
Vector::Vector(const Point& ipoint) : Vector(ipoint.x, ipoint.y) {}

/**
 * Vector Accessors
 */
QLength& Vector::operator[](size_t iindex) {
  switch (iindex) {
    case 0: return x; break;
    case 1: return y; break;
    default:
      throw std::runtime_error("Vector::at():: \"" + std::to_string(iindex) +
                               "\" is invalid index");
      break;
  }
}

const QLength& Vector::operator[](size_t iindex) const {
  return const_cast<Vector*>(this)->operator[](iindex);
}

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
 * Vector Functions
 */
Vector Vector::operator*(const double scalar) const {
  return {x * scalar, y * scalar};
}

Vector Vector::operator/(const double scalar) const {
  return {x / scalar, y / scalar};
}

QLength Vector::dist(const Vector& lhs, const Vector& rhs) {
  return meter *
         std::sqrt(
           ((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y)).convert(meter2));
}

} // namespace lib7842