#include "state.hpp"

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

/**
 * State Math Operators
 */
State State::operator+(const State& rhs) const {
  return {x + rhs.x, y + rhs.y, theta + rhs.theta};
}

State State::operator-(const State& rhs) const {
  return {x - rhs.x, y - rhs.y, theta - rhs.theta};
}

/**
 * State Equality Operators
 */
bool State::operator==(const State& rhs) const {
  return x == rhs.x && y == rhs.y && theta == rhs.theta;
}

bool State::operator!=(const State& rhs) const {
  return !(rhs == *this);
}

} // namespace lib7842