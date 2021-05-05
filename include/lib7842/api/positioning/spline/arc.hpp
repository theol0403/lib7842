#pragma once
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/other/utility.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "piecewise.hpp"
#include "spline.hpp"

namespace lib7842 {

/**
 * This class describes a arc which connects two points together while taking the average of the
 * angles.
 */
class Arc : public SplineHelper<Arc> {
public:
  constexpr Arc(const State& istart, const State& iend) : start(istart), end(iend) {
    theta = util::rollAngle180(iend.theta - istart.theta);
    c = start.distTo(end);

    if (theta == 0_rad) {
      r = NAN * meter;
      s = c;
    } else {
      r = c / (2.0 * sin(theta / 2.0));
      s = r / radian * theta;
    }

    rotate = start.angleTo(end) - theta / 2.0;
  }

  constexpr State calc(double t) const override {
    QLength x = 0_m;
    QLength y = 0_m;
    if (std::isnan(r.convert(meter))) {
      y = s * t;
    } else {
      x = r * cos(t * theta) - r;
      y = r * sin(t * theta);
    }

    QAngle new_theta = rotate - 90_deg;
    QLength x_r = x * cos(new_theta) - y * sin(new_theta);
    QLength y_r = y * cos(new_theta) + x * sin(new_theta);

    x_r += start.x;
    y_r += start.y;

    return {x_r, y_r, rotate + theta * t};
  }

  constexpr QCurvature curvature(double /*t*/) const override {
    if (std::isnan(r.convert(meter))) { return 0 / meter; }
    return 1.0 / r;
  }

  constexpr QLength length(double /*resolution*/ = 0) const override { return s; }

  constexpr Vector calc_d(double t) const {
    QLength x = 0_m;
    QLength y = 0_m;
    if (std::isnan(r.convert(meter))) {
      y = s;
    } else {
      x = r * theta / radian * -sin(t * theta);
      y = r * theta / radian * cos(t * theta);
    }

    QAngle new_theta = rotate - 90_deg;
    QLength x_r = x * cos(new_theta) - y * sin(new_theta);
    QLength y_r = y * cos(new_theta) + x * sin(new_theta);

    return {x_r, y_r};
  }

  constexpr Vector calc_d2(double t) const {
    QLength x = 0_m;
    QLength y = 0_m;
    if (!std::isnan(r.convert(meter))) {
      x = r * square(theta / radian) * -cos(t * theta);
      y = r * square(theta / radian) * -sin(t * theta);
    }

    QAngle new_theta = rotate - 90_deg;
    QLength x_r = x * cos(new_theta) - y * sin(new_theta);
    QLength y_r = y * cos(new_theta) + x * sin(new_theta);

    return {x_r, y_r};
  }

protected:
  Vector start; // the start point
  Vector end; // the end point
  QAngle theta; // the arc angle of the arc
  QLength c; // the chord length
  QLength r; // the arc radius
  QLength s; // the arc length
  QAngle rotate; // how much the arc should be rotated
};

/**
 * Helper function used to create a Piecewise<Arc> using an array of states. Connects an arc
 * between each point. The number of arcs in the piecewise is one less than the number of points.
 *
 * @tparam P Must be Arc.
 * @tparam N The number of points provided.
 * @return A Piecewise<P, N-1>>.
 */
template <class P, size_t N>
requires std::same_as<P, Arc>
constexpr auto make_piecewise(State(&&ip)[N]) {
  std::array<std::optional<P>, N - 1> p;
  for (size_t i = 0; i < N - 1; ++i) {
    p[i].emplace(ip[i], ip[i + 1]);
  }
  return Piecewise(std::move(p));
}

} // namespace lib7842
