#pragma once
#include "parametric.hpp"
#include "spline.hpp"

namespace lib7842 {

// forward declaration
template <size_t N> class BezierFnc;
// aliases
template <size_t N> using Bezier = Parametric<BezierFnc<N>>;
using CubicBezier = Bezier<3>;
using QuarticBezier = Bezier<4>;
using QuinticBezier = Bezier<5>;

/**
 * A BezierFnc is a one-dimensional function that uses N+1 control points to produce a function of N
 * order. The spline begins at the first control point, ends at the last, but does not pass through
 * any middle points. The tangent of the start and end points are equal to the angle to their
 * nearest respective points. The bezier function is calculated using a generic implementation that
 * works with a bezier of any order using the recursive notation of the binomial coefficient.
 * However, this is quite inefficient, so template specialization is required for good performance.
 * https://en.wikipedia.org/wiki/B%C3%A9zier_curve
 *
 * This class is designed to be used with the Parametric class to produce a two-dimensional spline.
 * Bezier<N> is an alias for Parametric<BezierFnc<N>>. There are also aliases for CubicBezier,
 * QuarticBezier, and QuinticBezier.
 *
 * @tparam N The order of the BezierFnc.
 */
template <size_t N> class BezierFnc : public ParametricFnc {
public:
  /**
   * Create a new one-dimensional BezierFnc<N> given an array of N+1 control points.
   *
   * @param ictrls The control points.
   */
  constexpr explicit BezierFnc(const std::array<double, N + 1>& ictrls) : ctrls(ictrls) {}

  /**
   * Calculate the y value of the bezier given x.
   * https://en.wikipedia.org/wiki/B%C3%A9zier_curve#Terminology
   *
   * @param  x The input value in the range of [0, 1].
   * @return The calculated y value.
   */
  constexpr double calc(double x) const override {
    double sum {0.0};
    for (size_t i = 0; i < ctrls.size(); ++i) {
      sum += basis(N, i, x) * ctrls[i];
    }
    return sum;
  }

  /**
   * Calculate the first derivative of the bezier given x.
   * https://en.wikipedia.org/wiki/B%C3%A9zier_curve#Derivative
   *
   * @param  x The input value in the range of [0, 1].
   * @return The calculated first derivative.
   */
  constexpr double calc_d(double x) const override {
    double sum {0.0};
    for (size_t i = 0; i < N; ++i) {
      sum += basis(N - 1, i, x) * N * (ctrls[i + 1] - ctrls[i]);
    }
    return sum;
  }

  /**
   * Calculate the second derivative of the bezier given x.
   *
   * @param  x The input value in the range of [0, 1].
   * @return The calculated second derivative.
   */
  constexpr double calc_d2(double x) const override {
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
   * https://en.wikipedia.org/wiki/Binomial_coefficient
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

// deduction guide to create BezierFnc given array of control points.
template <size_t N> BezierFnc(const std::array<double, N>&) -> BezierFnc<N - 1>;

/**
 * Specialization for a Parametric<BezierFnc<N>> which allows construction using an array of 2D
 * control points.
 */
template <size_t N> class Parametric<BezierFnc<N>> : public Parametric<BezierFnc<N>, true> {
public:
  /**
   * Helper method to construct a Piecewise<BezierFnc<N>> given an array of 2D control points.
   * Separates the x and y component of the control points and delegates them to their respective
   * functions.
   *
   * @param ctrls The array of control points.
   */
  constexpr explicit Parametric(const Vector (&ctrls)[N + 1]) :
    Parametric<BezierFnc<N>, true>(
      BezierFnc(process(ctrls, [](const auto& ip) { return ip.x.convert(meter); })),
      BezierFnc(process(ctrls, [](const auto& ip) { return ip.y.convert(meter); }))) {}

private:
  /**
   * Helper method to transform an array of 2D Vector into an array of 1D control points.
   */
  constexpr auto process(const Vector (&ctrls)[N + 1], auto&& f) {
    std::array<double, N + 1> t;
    std::transform(std::begin(ctrls), std::end(ctrls), std::begin(t), f);
    return t;
  }
};

// deduction guide to create Bezier given array of 2D control points.
template <size_t N> Parametric(const Vector (&)[N]) -> Parametric<BezierFnc<N - 1>>;

} // namespace lib7842
