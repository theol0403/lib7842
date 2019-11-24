#pragma once

#include "okapi/api/odometry/point.hpp"
#include "okapi/api/units/QArea.hpp"
#include "okapi/api/units/QLength.hpp"

namespace lib7842 {

using namespace okapi;

/**
 * A 2D Point
 */
class Vector {
public:
  QLength x {0_in};
  QLength y {0_in};

  Vector() = default;
  Vector(const Vector& ipoint) = default;
  virtual ~Vector() = default;

  /**
   * Create a new point
   *
   * @param ix The x
   * @param iy The y
   */
  Vector(const QLength& ix, const QLength& iy);

  /**
   * Convert a Point to a Vector
   *
   * @param ipoint The point
   */
  explicit Vector(const Point& ipoint);

  /**
   * Get the coordinate given an index. 0 is X, 1 is Y.
   *
   * @param  iindex The index
   * @return The coordinate
   */
  QLength& at(const size_t& iindex);
  QLength& operator[](const size_t& iindex);

  /**
   * Get the read-only coordinate given an index. 0 is X, 1 is Y.
   *
   * @param  iindex The index
   * @return The coordinate
   */
  const QLength& read(const size_t& iindex) const;

  /**
   * Binary operators
   */
  Vector operator+(const Vector& rhs) const;
  Vector operator-(const Vector& rhs) const;
  bool operator==(const Vector& rhs) const;
  bool operator!=(const Vector& rhs) const;

  /**
   * Scale operators
   */
  Vector operator*(const double scalar) const;
  Vector operator/(const double scalar) const;

  /**
   * Vector operations
   */
  static Vector normalize(const Vector& lhs);
  static Vector scalarMult(const Vector& lhs, const double scalar);
  static QArea dot(const Vector& lhs, const Vector& rhs);
  static QLength mag(const Vector& lhs);
  static QLength dist(const Vector& lhs, const Vector& rhs);
};

} // namespace lib7842