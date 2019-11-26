#include "odomMath.hpp"

namespace lib7842::OdomMath {

Vector closest(const Vector& current, const QAngle& heading, const Vector& target) {
  double headRad = heading.convert(radian);
  MathPoint n = MathPoint::normalize({sin(headRad), cos(headRad)});
  Vector v = target - current;
  double d = MathPoint::dot(v, n);
  return current + (n * d);
}

Vector closest(const State& state, const Vector& target) {
  return closest(state, state.theta, target);
}

QAngle rollAngle360(const QAngle& angle) {
  return angle - 360.0_deg * std::floor(angle.convert(degree) / 360.0);
}

QAngle rollAngle180(const QAngle& angle) {
  return angle - 360.0_deg * std::floor((angle.convert(degree) + 180.0) / 360.0);
}

QAngle rotateAngle90(const QAngle& angle) {
  QAngle iangle = rollAngle180(angle);
  if (iangle.abs() > 90_deg) {
    iangle += 180_deg;
    iangle = rollAngle180(iangle);
  }
  return iangle;
}

} // namespace lib7842::OdomMath
