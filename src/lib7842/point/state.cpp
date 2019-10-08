#include "state.hpp"

namespace lib7842 {

/**
 * QState Constructor
 */
QState::QState(const QLength& ix, const QLength& iy, const QAngle& itheta) :
  QPoint(ix, iy), theta(itheta) {}

/**
 * QState Constructor 
 * Upcast QPoint 
 */
QState::QState(const QPoint& ipoint) : QPoint(ipoint) {};

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