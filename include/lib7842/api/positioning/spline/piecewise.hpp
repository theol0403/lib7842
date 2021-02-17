#pragma once
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "spline.hpp"
#include <numeric>

namespace lib7842 {

/**
 * A Piecewise is a collection of splines that are connected together to form a larger spline. It is
 * sampled using `t` which is spread out across the range of the piecewise.
 *
 * @tparam S The type of spline used for the piecewise.
 * @tparam N The number of splines in the piecewise.
 */
template <class S, size_t N> class Piecewise : public SplineHelper<Piecewise<S, N>> {
public:
  /**
   * Construct a new Piecewise given an array of splines. This constructor is not intended to be
   * used directly, use `make_piecewise` instead. This is because the class template parameters
   * cannot be partially deduced, so a template function is needed to remove the need for explicit S
   * and N annotations.
   *
   * @param ip The array of splines.
   */
  constexpr explicit Piecewise(S(&&ip)[N]) {
    std::move(std::begin(ip), std::end(ip), std::begin(p));
  }

  /**
   * Construct a new Piecewise given an array of splines. This constructor is not intended to be
   * used directly, use `make_piecewise` instead.
   *
   * @param ip The array of splines.
   */
  constexpr explicit Piecewise(std::array<std::optional<S>, N>&& ip) : p(ip) {}

  /**
   * Provides all the necessary Spline overrides. These work by mapping `t` over the range of the
   * entire piecewise. This handles the knots (intersections) of the individual splines by choosing
   * the beginning of the next spline over the end of the current spline (unless the current spline
   * is the last one in the piecewise).
   *
   * @param  t Where along the spline to sample, in the range of [0, 1].
   * @return The sampled point at t.
   */
  constexpr State calc(double t) const override { return get(t, &S::calc); }
  constexpr QCurvature curvature(double t) const override { return get(t, &S::curvature); }
  constexpr QLength velocity(double t) const override { return get(t, &S::velocity) * N; }
  constexpr QLength length(double resolution) const override {
    return std::accumulate(std::begin(p), std::end(p), 0_m, [&](const QLength& l, const auto& ip) {
      return l + ip.value().length(resolution);
    });
  }

protected:
  std::array<std::optional<S>, N> p;

  /**
   * Helper function to map the value of t over the range of the piecewise. For example, if there
   * are 4 splines (5 points), and t is 0.5, then i should be 2 (third point) and x should be 0.
   *
   * @param  t Where along the spline to sample, in the range of [0, 1].
   * @param  f What value to get from the spline.
   * @return The value sampled from one of the splines according to t and f.
   */
  constexpr auto get(double t, const auto& f) const {
    size_t i = t * N; // which arc to use
    // use t = 1 for the last arc
    if (i == N) { i = N - 1; }
    double x = t * N - i; // which t to use
    return std::invoke(f, p[i].value(), x);
  }
};

/**
 * Helper function to construct a piecewise. Use this instead of the Piecewise constructors, as it
 * allows you to omit some template parameters in favor of deduction. There are overloads of this
 * function to provide extra helpers depending on the type of S, for example helpers to create a
 * piecewise Hermite or Line.
 *
 * @param  ip The array of splines.
 * @tparam S  The type of spline. Can sometimes be omitted if the type is known for ip.
 * @tparam N  The number of splines. Can always be omitted, which is why this helper exists.
 * @return A Piecewise<S, N>.
 */
template <class S, size_t N> constexpr auto make_piecewise(S(&&ip)[N]) {
  return Piecewise<S, N>(std::move(ip));
}

} // namespace lib7842
