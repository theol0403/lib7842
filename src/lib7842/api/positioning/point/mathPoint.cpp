#include "lib7842/api/positioning/point/mathPoint.hpp"

namespace lib7842 {

MathPoint::MathPoint(double ix, double iy) : x(ix), y(iy) {}

MathPoint::MathPoint(const Vector& ipoint) :
  MathPoint(ipoint.x.convert(meter), ipoint.y.convert(meter)) {}

MathPoint::operator Vector() const {
  return {x * meter, y * meter};
}

MathPoint MathPoint::operator+(const MathPoint& rhs) const {
  return {x + rhs.x, y + rhs.y};
}

MathPoint MathPoint::operator-(const MathPoint& rhs) const {
  return {x - rhs.x, y - rhs.y};
}

bool MathPoint::operator==(const MathPoint& rhs) const {
  return x == rhs.x && y == rhs.y;
}

bool MathPoint::operator!=(const MathPoint& rhs) const {
  return !(rhs == *this);
}

MathPoint MathPoint::operator*(const double scalar) const {
  return {x * scalar, y * scalar};
}

MathPoint MathPoint::operator/(const double scalar) const {
  return {x / scalar, y / scalar};
}

double MathPoint::dist(const MathPoint& lhs, const MathPoint& rhs) {
  return std::sqrt((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y));
}

MathPoint MathPoint::normalize(const MathPoint& point) {
  double imag = mag(point);
  return imag ? point / imag : point;
}

double MathPoint::dot(const MathPoint& lhs, const MathPoint& rhs) {
  return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}

double MathPoint::mag(const MathPoint& point) {
  return std::sqrt(point.x * point.x + point.y * point.y);
}

} // namespace lib7842