#pragma once
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include "stepper.hpp"

namespace lib7842 {

/**
 * Base spline class. Main method of sampling is via calc, according to a t parameter that is in the
 * range of [0, 1].
 */
class Spline {
public:
  /**
   * Sample the point along the spline given t.
   *
   * @param  t Where along the spline to sample, in the range of [0, 1]
   * @return the sampled point at t
   */
  constexpr virtual State calc(double t) const = 0;

  /**
   * Sample the curvature of the spline at t. Curvature is the inverse of the radius.
   *
   * @param  t Where along the spline to sample, in the range of [0, 1]
   * @return the curvature at t
   */
  constexpr virtual QCurvature curvature(double t) const = 0;

  /**
   * Sample the velocity of the spline at t. Velocity is the ratio between distance traveled and
   * change in t.
   *
   * @param t Where along the spline to sample, in the range of [0, 1]
   * @return the velocity at t
   */
  constexpr virtual QLength velocity(double /*t*/) const { return length(); }

  /**
   * Calculate the length of the spline. This method has a default implementation that tries to fit
   * lines onto the spline and sums their length.
   *
   * @param  resolution The number of lines to fit to the spline.
   * @return the length of the spline
   */
  constexpr virtual QLength length(double resolution = 100) const {
    QLength len {0.0};
    for (size_t i = 0; i < resolution; i++) {
      len += calc(i / resolution).distTo(calc((i + 1) / resolution));
    }
    return len;
  }

  /**
   * Using the velocity of the spline, calculate how much to increment t to travel a certain
   * distance.
   *
   * @param  t    The previous value of t.
   * @param  dist The desired distance to travel.
   * @return the best estimate of the new value of t
   */
  constexpr virtual double t_at_dist_travelled(double t, const QLength& dist) const {
    return t + (dist / velocity(t).abs()).convert(number);
  }
};

/**
 * Provides some additional spline methods that require knowledge of the derived class type. This is
 * solved using a CRTP. All splines should inherit from this class rather than Spline.
 *
 * @tparam CRTP The derived class type.
 */
template <class CRTP> class SplineHelper : public Spline {
public:
  /**
   * Return a Stepper that contains a reference to the spline and a given StepBy.
   */
  template <class S> constexpr auto step(S&& s) const& {
    return Stepper(static_cast<const CRTP&>(*this), std::forward<S>(s));
  }
  template <class S> constexpr auto step(S&& s) && {
    return Stepper(static_cast<CRTP&&>(*this), std::forward<S>(s));
  }

  /**
   * Generate the spline given a StepBy. Generate means to sample the whole spline and return an
   * array of points.
   */
  template <class S> auto generate(S&& s) const& {
    return Stepper(static_cast<const CRTP&>(*this), std::forward<S>(s)).generate();
  }
  template <class S> auto generate(S&& s) && {
    return Stepper(static_cast<CRTP&&>(*this), std::forward<S>(s)).generate();
  }
};
} // namespace lib7842
