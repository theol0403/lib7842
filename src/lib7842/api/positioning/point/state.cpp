#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/other/utility.hpp"

namespace lib7842 {

/**
 * State Constructor
 */
State::State(const QLength& ix, const QLength& iy, const QAngle& itheta) :
  Vector(ix, iy), theta(itheta) {}

/**
 * State Constructor
 * Upcast Vector
 */
State::State(const Vector& ipoint) : Vector(ipoint) {};
State::State(const Vector& ipoint, const QAngle& itheta) : Vector(ipoint), theta(itheta) {};

/**
 * State Constructor
 * Upcast OdomState
 */
State::State(const OdomState& ipoint) : State(ipoint.x, ipoint.y, ipoint.theta) {};

State State::operator+(const State& rhs) const {
  return {x + rhs.x, y + rhs.y, theta + rhs.theta};
}

State State::operator-(const State& rhs) const {
  return {x - rhs.x, y - rhs.y, theta - rhs.theta};
}

State State::operator*(const double scalar) const {
  return {x * scalar, y * scalar, theta * scalar};
}

State State::operator/(const double scalar) const {
  return {x / scalar, y / scalar, theta / scalar};
}

bool State::operator==(const State& rhs) const {
  return x == rhs.x && y == rhs.y && theta == rhs.theta;
}

bool State::operator!=(const State& rhs) const {
  return !(rhs == *this);
}

QAngle State::angleTo(const Vector& ipoint) const {
  return util::rollAngle180(Vector::angleTo(ipoint) - theta);
}

} // namespace lib7842
