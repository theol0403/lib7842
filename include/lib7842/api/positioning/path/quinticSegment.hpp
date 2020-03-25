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
   * @param  t the polynomial input
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
class QuinticSegment : public PathHelper<QuinticSegment> {
public:
  /**
   * Create a spline from two points, both containing a position, angle, and slope. Slope, which is
   * the importance of a point's angle, must be provided as data named "slope".
   *
   * @param istart The start point
   * @param iend   The end point
   */
  QuinticSegment(DataState istart, DataState iend);

  /**
   * Interpolate the path
   *
   * @param  isteps How many points to interpolate per segment, counting from the start to just
   *                before the end of the segment. This means is 1 step will return the first point
   *                and 2 steps will return the first point as well as a midway point. The end point
   *                is not included in the count.
   * @param  iend   Whether to return the end of the segment. This can be turned off to prevent the
   *                start of the next segment from being redundant.
   * @return generated path
   */
  SimplePath generate(int isteps = 1, bool iend = true) const override;

protected:
  DataState start {};
  DataState end {};
};

} // namespace lib7842
