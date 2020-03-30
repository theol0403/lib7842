#include "lib7842/api/positioning/point/vector.hpp"
#include "lib7842/api/other/global.hpp"
#include "lib7842/api/other/utility.hpp"
#include "okapi/api/units/QArea.hpp"
#include <stdexcept>
#include <string>

namespace lib7842 {

Vector::Vector(const QLength& ix, const QLength& iy) : x(ix), y(iy) {}

Vector::Vector(const Point& ipoint) : Vector(ipoint.x, ipoint.y) {}

QLength& Vector::at(size_t iindex) {
  switch (iindex) {
    case 0: return x;
    case 1: return y;
    default:
      GLOBAL_ERROR_THROW("Vector::at():: \"" + std::to_string(iindex) + "\" is invalid index");
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

Vector Vector::operator*(double scalar) const {
  return {x * scalar, y * scalar};
}

Vector Vector::operator/(double scalar) const {
  return {x / scalar, y / scalar};
}

bool Vector::operator==(const Vector& rhs) const {
  return x == rhs.x && y == rhs.y;
}

bool Vector::operator!=(const Vector& rhs) const {
  return !(rhs == *this);
}

QLength Vector::dist(const Vector& lhs, const Vector& rhs) {
  return meter *
         std::sqrt(
           ((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y)).convert(meter2));
}

QLength Vector::distTo(const Vector& ipoint) const {
  return dist(*this, ipoint);
}

QAngle Vector::angle(const Vector& istart, const Vector& iend) {
  Vector diff = iend - istart;
  QAngle angle = (std::atan2(diff.x.convert(meter), diff.y.convert(meter)) * radian);
  return util::rollAngle180(angle);
}

QAngle Vector::angleTo(const Vector& ipoint) const {
  return angle(*this, ipoint);
}

} // namespace lib7842
