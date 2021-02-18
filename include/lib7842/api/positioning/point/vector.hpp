#pragma once
#include "lib7842/api/other/global.hpp"
#include "lib7842/api/other/utility.hpp"
#include "okapi/api/odometry/point.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "okapi/api/units/QArea.hpp"
#include "okapi/api/units/QLength.hpp"

namespace lib7842 {
using namespace okapi;

/**
 * A 2D Point
 */
struct Vector {
  QLength x {0_in};
  QLength y {0_in};

  constexpr Vector() = default;
  constexpr ~Vector() = default;

  /**
   * Create a new point
   *
   * @param ix The x
   * @param iy The y
   */
  constexpr Vector(const QLength& ix, const QLength& iy) : x(ix), y(iy) {}

  /**
   * Convert a Point to a Vector
   *
   * @param ipoint The point
   */
  constexpr explicit Vector(const okapi::Point& ipoint) : Vector(ipoint.x, ipoint.y) {}

  /**
   * Get the coordinate given an index. 0 is X, 1 is Y.
   *
   * @param  iindex The index
   * @return The coordinate
   */
  constexpr QLength& at(size_t iindex) {
    switch (iindex) {
      case 0: return x;
      case 1: return y;
      default:
        GLOBAL_ERROR_THROW("Vector::at():: \"" + std::to_string(iindex) + "\" is invalid index");
    }
  }

  constexpr const QLength& at(size_t iindex) const { return const_cast<Vector*>(this)->at(iindex); }

  /**
   * Operators
   */
  constexpr Vector operator+(const Vector& rhs) const { return {x + rhs.x, y + rhs.y}; }
  constexpr Vector operator-(const Vector& rhs) const { return {x - rhs.x, y - rhs.y}; }
  constexpr Vector operator*(double scalar) const { return {x * scalar, y * scalar}; }
  constexpr Vector operator/(double scalar) const { return {x / scalar, y / scalar}; }
  constexpr bool operator==(const Vector& rhs) const { return x == rhs.x && y == rhs.y; }
  constexpr bool operator!=(const Vector& rhs) const { return !(*this == rhs); }

  /**
   * Calculate distance between points
   */
  constexpr static QLength dist(const Vector& lhs, const Vector& rhs) {
    return sqrt((square(lhs.x - rhs.x) + square(lhs.y - rhs.y)));
  }

  constexpr QLength distTo(const Vector& ipoint) const { return dist(*this, ipoint); }

  /**
   * Calculate angle between points
   */
  constexpr static QAngle angle(const Vector& istart, const Vector& iend) {
    Vector diff = iend - istart;
    QAngle angle = atan2(diff.x, diff.y);
    return util::rollAngle180(angle);
  }

  constexpr QAngle angleTo(const Vector& ipoint) const { return angle(*this, ipoint); }

protected:
  friend inline std::ostream& operator<<(std::ostream& os, const Vector& rhs) {
    os << "{" << rhs.x << ", " << rhs.y << "}";
    return os;
  }
};
} // namespace lib7842
