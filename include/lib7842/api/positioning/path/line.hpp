#pragma once
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "path.hpp"

namespace lib7842 {

/**
 * This class describes a line which connects two points together.
 */
class Line : public PathHelper<Line> {
public:
  /**
   * Create a new line given the coordinates of two points.
   *
   * @param istart The start point
   * @param iend   The end point
   */
  constexpr Line(const Vector& istart, const Vector& iend) :
    start(istart, istart.angleTo(iend)), end(iend, start.theta) {}
  constexpr ~Line() override = default;

  /**
   * Sample the point along the path which is between the start and end points using linear
   * interpolation given t.
   *
   * @param  t The ratio of interpolation between the two points in the range of [0, 1].
   * @return The interpolated point.
   */
  constexpr State calc(double t) const override { return start + (end - start) * t; }

  /**
   * The curvature of a line is always zero.
   */
  constexpr QCurvature curvature(double /*t*/) const override { return 0 / meter; }

  /**
   * Calculate the length of the line which is the distance between the start and end points.
   */
  constexpr QLength length(double /*resolution*/ = 0) const override { return start.distTo(end); }

protected:
  const State start;
  const State end;
};

} // namespace lib7842