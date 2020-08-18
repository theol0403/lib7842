#pragma once
#include "vector.hpp"

#include "okapi/api/odometry/odomState.hpp"
#include "okapi/api/units/QAngle.hpp"

namespace lib7842 {

/**
 * A 2D point with a third theta member
 */
struct State : public Vector {
  QAngle theta {0_rad};

  constexpr State() = default;
  constexpr ~State() = default;

  /**
   * Create a new state
   *
   * @param ix The x
   * @param iy The y
   * @param itheta The theta
   */
  constexpr State(const QLength& ix, const QLength& iy, const QAngle& itheta) :
    Vector(ix, iy), theta(itheta) {}

  /**
   * Convert a Vector to a State
   *
   * @param ipoint The point
   */
  constexpr explicit State(const Vector& ipoint) : Vector(ipoint) {};

  /**
   * Convert a Vector to a State with an angle
   *
   * @param ipoint The point
   * @param itheta The angle
   */
  constexpr State(const Vector& ipoint, const QAngle& itheta) : Vector(ipoint), theta(itheta) {};

  /**
   * Convert a OdomState to a State
   *
   * @param ipoint The point
   */
  constexpr explicit State(const OdomState& ipoint) : State(ipoint.x, ipoint.y, ipoint.theta) {};

  /**
   * Binary operators
   */
  constexpr State operator+(const State& rhs) const {
    return {x + rhs.x, y + rhs.y, theta + rhs.theta};
  }
  constexpr State operator-(const State& rhs) const {
    return {x - rhs.x, y - rhs.y, theta - rhs.theta};
  }
  constexpr State operator*(double scalar) const {
    return {x * scalar, y * scalar, theta * scalar};
  }
  constexpr State operator/(double scalar) const {
    return {x / scalar, y / scalar, theta / scalar};
  }
  constexpr bool operator==(const State& rhs) const {
    return x == rhs.x && y == rhs.y && theta == rhs.theta;
  }
  constexpr bool operator!=(const State& rhs) const { return !(*this == rhs); }

  /**
   * Calculate angle from the state to a point
   *
   * @param  ipoint The point
   */
  constexpr QAngle angleTo(const Vector& ipoint) const {
    return util::rollAngle180(Vector::angleTo(ipoint) - theta);
  }

protected:
  friend inline std::ostream& operator<<(std::ostream& os, const State& rhs) {
    os << "{" << rhs.x << ", " << rhs.y << ", " << rhs.theta << "}";
    return os;
  }
};
} // namespace lib7842
