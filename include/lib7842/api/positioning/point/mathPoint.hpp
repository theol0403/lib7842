#pragma once
#include "vector.hpp"

namespace lib7842 {

/**
 * A 2D Point which is used for math operations
 */
struct MathPoint {
  double x {0}; // in meters
  double y {0}; // in meters

  MathPoint() = default;

  /**
   * Create a new point
   *
   * @param ix The x
   * @param iy The y
   */
  MathPoint(double ix, double iy);

  /**
   * Convert a Point to a MathPoint
   *
   * @param ipoint The point
   */
  MathPoint(const Vector& ipoint);

  /**
   * Convert a MathPoint to a Vector
   */
  operator Vector() const;

  /**
   * Binary operators
   */
  MathPoint operator+(const MathPoint& rhs) const;
  MathPoint operator-(const MathPoint& rhs) const;
  bool operator==(const MathPoint& rhs) const;
  bool operator!=(const MathPoint& rhs) const;

  /**
   * Scale operators
   */
  MathPoint operator*(double scalar) const;
  MathPoint operator/(double scalar) const;

  /**
   * Utility functions
   */
  static double dist(const MathPoint& lhs, const MathPoint& rhs);

  /**
   * MathPoint operations
   */
  static MathPoint normalize(const MathPoint& point);
  static double dot(const MathPoint& lhs, const MathPoint& rhs);
  static double mag(const MathPoint& point);
};
} // namespace lib7842
