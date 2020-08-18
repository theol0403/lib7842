#pragma once
#include "path.hpp"
#include <ranges>

namespace lib7842 {

// forward declaration
template <size_t N> class Bezier;
using CubicBezier = Bezier<3>;
using QuarticBezier = Bezier<4>;
using QuinticBezier = Bezier<5>;

/**
 * A Bezier is a one-dimensional function that uses N+1 number of control points to produce a
 * function of N order. The path begins at the first control point, ends at the last, but does not
 * pass through any middle points. The tangent of the start and end points are equal to the angle to
 * their nearest respective points.
 *
 * This is designed to be used with the Parametric class to produce a two-dimensional path. There
 * are aliases for a CubicBezier, QuarticBezier, and QuinticBezier.
 *
 * @tparam N The order of the Bezier.
 */
template <size_t N> class Bezier {
public:
  /**
   * Create a new bezier given an array of N+1 control points.
   *
   * @param ictrls The control points.
   */
  constexpr explicit Bezier(const std::array<double, N + 1>& ictrls) : ctrls(ictrls) {}
  constexpr ~Bezier() = default;

  /**
   * Calculate the y value of the bezier given x.
   *
   * @param  x The input value of the bezier in the range of [0, 1].
   * @return The calculated y value of the bezier.
   */
  constexpr double calc(double x) const {
    double sum {0.0};
    for (size_t i = 0; i < ctrls.size(); ++i) {
      sum += basis(N, i, x) * ctrls[i];
    }
    return sum;
  }

  /**
   * Calculate the first derivative of the bezier given x.
   *
   * @param  x The input value of the bezier in the range of [0, 1].
   * @return The calculated first derivative of the bezier.
   */
  constexpr double calc_d(double x) const {
    double sum {0.0};
    for (size_t i = 0; i < N; ++i) {
      sum += basis(N - 1, i, x) * N * (ctrls[i + 1] - ctrls[i]);
    }
    return sum;
  }

  /**
   * Calculate the second derivative of the bezier given x.
   *
   * @param  x The input value of the bezier in the range of [0, 1].
   * @return The calculated second derivative of the bezier.
   */
  constexpr double calc_d2(double x) const {
    double sum {0.0};
    for (size_t i = 0; i < N - 1; ++i) {
      sum += basis(N - 2, i, x) * N * (N - 1) * (ctrls[i + 2] - 2 * ctrls[i + 1] + ctrls[i]);
    }

    return sum;
  }

  static constexpr size_t order = N;

protected:
  std::array<double, N + 1> ctrls {};

  /**
   * The bezier basis function.
   */
  static constexpr double basis(size_t n, size_t k, double x) {
    return comb(n, k) * std::pow(1.0 - x, n - k) * std::pow(x, k);
  }

  /**
   * The binomial coefficient function, also known as the k-comb algorithm.
   */
  static constexpr size_t comb(size_t n, size_t k) {
    if (k > n) { return 0; }
    if (k > n - k) { k = n - k; }
    if (k == 0 or n <= 1) { return 1; }
    return factorial(n) / (factorial(k) * factorial(n - k));
  }

  /**
   * Naive factorial implementation.
   */
  static constexpr size_t factorial(size_t n) {
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
  }
};

template <size_t N> Bezier(const std::array<double, N>&) -> Bezier<N - 1>;

} // namespace lib7842