#pragma once
#include "abstractPath.hpp"
#include "discretePath.hpp"
#include "lib7842/api/positioning/point/data.hpp"

namespace lib7842 {

/**
 * A 1d quintic polynomial
 */
class QuinticPolynomial {
public:
  /**
   * Create a new polynomial
   *
   * @param istart      The start value
   * @param istartSlope The start slope
   * @param iend        The end value
   * @param iendSlope   The end slope
   */
  QuinticPolynomial(double istart, double istartSlope, double iend, double iendSlope);

  /**
   * Calculate the value of the polynomial at t
   *
   * @param  t
   * @return the value
   */
  double calculate(double t) const;

protected:
  double start {};
  double startSlope {};
  double end {};
  double endSlope {};
};

/**
 * A path that represents a quintic spline.
 */
class QuinticSegment : public TemplatePath<QuinticSegment> {
public:
  /**
   * Create a spline from two points, containing a position, heading, and slope.
   *
   * @param istart The start point
   * @param iend   The end point
   */
  QuinticSegment(const DataState& istart, const DataState& iend);

  /**
   * Interpolate the path
   *
   * @param  isteps how many points to interpolate per segment, from start (inclusive) to end
   *                (exclusive) of segment.
   * @return generated path
   */
  SimplePath generate(int isteps = 1) const override;

protected:
  DataState start {};
  DataState end {};
};

} // namespace lib7842