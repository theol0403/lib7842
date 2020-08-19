#pragma once
#include "path.hpp"
#include <cstddef>
#include <ranges>

namespace lib7842 {

// forward declaration
template <size_t N> class Hermite;
using CubicHermite = Hermite<3>;
using QuinticHermite = Hermite<5>;

/**
 * A Hermite is a one-dimensional function that uses two points and two tangents to produce a
 * function of any order. The path begins at the first control point with the first tangent and ends
 * at the last point with the last tangent. By scaling the tangents, the "stretch" parameter of the
 * function can be controlled, which is essentially the weighting of the tangents.
 * https://en.wikipedia.org/wiki/Cubic_Hermite_spline
 *
 * This class is designed to be used with the Parametric class to produce a two-dimensional path.
 * There are aliases for CubicHermite and QuinticHermite, which are the only two orders currently
 * supported. The hermite function is calculated using a generic implementation that only requires
 * the coefficients of x^i to be solved ahead of time.
 *
 * @tparam N The order of the Hermite.
 */
template <size_t N> class Hermite {
public:
  /**
   * Create a new one-dimensional Hermite given a start and end value and their tangents.
   */
  constexpr Hermite(double start, double start_t, double end, double end_t);
  constexpr ~Hermite() = default;

  /**
   * Calculate the y value of the hermite given x.
   *
   * @param  x The input value in the range of [0, 1].
   * @return The calculated y value.
   */
  constexpr double calc(double x) const {
    double sum {0};
    for (size_t i = 0; i < coeffs.size(); ++i) {
      sum += coeffs[i] * std::pow(x, i);
    }
    return sum;
  }

  /**
   * Calculate the first derivative of the hermite given x using the power rule.
   *
   * @param  x The input value in the range of [0, 1].
   * @return The calculated first derivative.
   */
  constexpr double calc_d(double x) const {
    double sum {0};
    for (size_t i = 1; i < coeffs.size(); ++i) {
      sum += coeffs[i] * i * std::pow(x, i - 1);
    }
    return sum;
  }

  /**
   * Calculate the second derivative of the hermite given x using the power rule.
   *
   * @param  x The input value in the range of [0, 1].
   * @return The calculated second derivative.
   */
  constexpr double calc_d2(double x) const {
    double sum {0};
    for (size_t i = 2; i < coeffs.size(); ++i) {
      sum += coeffs[i] * i * (i - 1) * std::pow(x, i - 2);
    }
    return sum;
  }

  static constexpr size_t order = N;

protected:
  std::array<double, N + 1> coeffs {};
};

/**
 * Template specializations for a cubic and quintic hermite.
 */
template <> constexpr Hermite<3>::Hermite(double start, double start_t, double end, double end_t) {
  double u = end - start;

  double a3 = 3.0 * u - 2.0 * start_t - end_t;
  double a4 = -2.0 * u + start_t + end_t;

  coeffs = {start, start_t, a3, a4};
}

template <> constexpr Hermite<5>::Hermite(double start, double start_t, double end, double end_t) {
  double u = end - start - start_t;
  double v = end_t - start_t;

  double a3 = 10.0 * u - 4.0 * v;
  double a4 = -15.0 * u + 7.0 * v;
  double a5 = 6.0 * u - 3.0 * v;

  coeffs = {start, start_t, 0.0, a3, a4, a5};
}

} // namespace lib7842