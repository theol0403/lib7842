#include "odomMath.hpp"

namespace lib7842 {

namespace lib7842::OdomMath {

// dPoint closest(const dPoint& current, const dPoint& head, const dPoint& target) {
//   dPoint n = normalize(head);
//   dPoint v = sub(target, current);
//   double d = dot(v.x, v.y, n.x, n.y);
//   return add(current, multScalar(n, d));
// }

// Vector closest(const Vector& current, const Vector& target) {
//   return closest(
//     current, dPoint {sin(current.theta.convert(radian)), cos(current.theta.convert(radian))},
//     target);
// }

QAngle rollAngle360(const QAngle& angle) {
  return angle - 360.0_deg * std::floor(angle.convert(degree) * (1.0 / 360.0));
}

QAngle rollAngle180(const QAngle& angle) {
  return angle - 360.0_deg * std::floor((angle.convert(degree) + 180.0) * (1.0 / 360.0));
}

QAngle rollAngle90(const QAngle& angle) {
  angle = rollAngle180(angle);
  if (angle.abs() > 90_deg) {
    angle += 180_deg;
    angle = rollAngle180(angle);
  }
  return angle;
}

} // namespace lib7842::OdomMath

} // namespace lib7842
