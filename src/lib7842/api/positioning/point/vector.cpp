#include "vector.hpp"
#include "okapi/api/units/QArea.hpp"
#include <stdexcept>
#include <string>

namespace lib7842 {

Vector::Vector(const QLength& ix, const QLength& iy) : x(ix), y(iy) {}

Vector::Vector(const Point& ipoint) : Vector(ipoint.x, ipoint.y) {}

QLength& Vector::at(size_t iindex) {
  switch (iindex) {
    case 0: return x; break;
    case 1: return y; break;
    default:
      throw std::runtime_error("Vector::at():: \"" + std::to_string(iindex) +
                               "\" is invalid index");
      break;
  }
}

const QLength& Vector::at(size_t iindex) const {
  return const_cast<Vector*>(this)->at(iindex);
}

Vector Vector::operator+(const Vector& rhs) const {
  return {x + rhs.x, y + rhs.y};
}

Vector Vector::operator-(const Vector& rhs) const {
  return {x - rhs.x, y - rhs.y};
}

bool Vector::operator==(const Vector& rhs) const {
  return x == rhs.x && y == rhs.y;
}

bool Vector::operator!=(const Vector& rhs) const {
  return !(rhs == *this);
}

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

QLength Vector::distTo(const Vector& ipoint) {
  return dist(*this, ipoint);
}

} // namespace lib7842