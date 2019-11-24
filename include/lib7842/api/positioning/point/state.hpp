#pragma once
#include "vector.hpp"

#include "okapi/api/odometry/odomState.hpp"
#include "okapi/api/units/QAngle.hpp"

namespace lib7842 {

/**
 * A 2D point with a third theta member
 */
class State : public Vector {
public:
  QAngle theta {0_rad};

  using Vector::Vector;
  State(const State& istate) = default;
  virtual ~State() = default;

  /**
   * Create a new state
   *
   * @param ix The x
   * @param iy The y
   * @param itheta The theta
   */
  State(const QLength& ix, const QLength& iy, const QAngle& itheta);

  /**
   * Convert a Vector to a State
   *
   * @param ipoint The point
   */
  explicit State(const Vector& ipoint);

  /**
   * Convert a OdomState to a State
   *
   * @param ipoint The point
   */
  explicit State(const OdomState& ipoint);

  /**
   * Binary operators
   */
  State operator+(const State& rhs) const;
  State operator-(const State& rhs) const;
  bool operator==(const State& rhs) const;
  bool operator!=(const State& rhs) const;
};

} // namespace lib7842