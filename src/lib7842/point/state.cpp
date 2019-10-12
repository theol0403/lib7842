#include "state.hpp"

namespace lib7842 {

/**
 * QState Constructor
 */
QState::QState(const QLength& ix, const QLength& iy, const QAngle& itheta) :
  Vector(ix, iy), theta(itheta) {}

/**
 * QState Constructor 
 * Upcast Vector 
 */
QState::QState(const Vector& ipoint) : Vector(ipoint) {};

/**
 * QState Math Operators
 */
QState QState::operator+(const QState& rhs) const {
  return {x + rhs.x, y + rhs.y, theta + rhs.theta};
}

QState QState::operator-(const QState& rhs) const {
  return {x - rhs.x, y - rhs.y, theta - rhs.theta};
}

/**
 * QState Equality Operators
 */
bool QState::operator==(const QState& rhs) const {
  return x == rhs.x && y == rhs.y && theta == rhs.theta;
}

bool QState::operator!=(const QState& rhs) const {
  return !(rhs == *this);
}

} // namespace lib7842