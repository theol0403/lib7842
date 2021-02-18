#pragma once
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "piecewise.hpp"
#include "spline.hpp"

namespace lib7842 {

/**
 * This class describes a line which connects two points together.
 */
class Line : public SplineHelper<Line> {
public:
  /**
   * Create a new line given the coordinates of two points.
   *
   * @param istart The start point.
   * @param iend   The end point.
   */
  constexpr Line(const Vector& istart, const Vector& iend) :
    start(istart, istart.angleTo(iend)), end(iend, start.theta) {}

  /**
   * Sample the point along the spline which is between the start and end points using linear
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
  State start;
  State end;
};

/**
 * Helper function used to create a Piecewise<Line> using an array of points. Connects a line
 * between each point. The number of lines in the piecewise is one less than the number of points.
 *
 * @tparam P Must be Line.
 * @tparam N The number of points provided.
 * @return A Piecewise<P, N-1>>.
 */
template <class P, size_t N>
requires std::same_as<P, Line> constexpr auto make_piecewise(Vector(&&ip)[N]) {
  std::array<std::optional<P>, N - 1> p;
  for (size_t i = 0; i < N - 1; ++i) {
    p[i].emplace(ip[i], ip[i + 1]);
  }
  return Piecewise(std::move(p));
}

} // namespace lib7842
