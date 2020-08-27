#pragma once
#include "parametric.hpp"
#include "path.hpp"

namespace lib7842 {

// forward declaration
template <size_t N> class HermiteFnc;
// aliases
template <size_t N> using Hermite = Parametric<HermiteFnc<N>>;
using CubicHermite = Hermite<3>;
using QuinticHermite = Hermite<5>;

/**
 * A HermiteFnc is a one-dimensional function that uses two points and two tangents to produce a
 * function of any order. The function begins at the first control point with the first tangent and
 * ends at the last point with the last tangent. By scaling the tangents, the "stretch" parameter of
 * the function can be controlled, which is essentially the weight of the tangents. The hermite
 * function is calculated using a generic implementation that only requires the coefficients of x^i
 * to be solved ahead of time. https://en.wikipedia.org/wiki/Cubic_Hermite_spline
 *
 * This class is designed to be used with the Parametric class to produce a two-dimensional spline.
 * Hermite<N> is an alias for Parametric<HermiteFnc<N>>. There are also aliases for CubicHermite and
 * QuinticHermite which are the only orders currently supported.
 *
 * @tparam N The order of the Hermite.
 */
template <size_t N> class HermiteFnc : public ParametricFnc {
public:
  /**
   * Create a new one-dimensional HermiteFnc given a start and end value and their tangents.
   *
   * @param start   The starting value of the function.
   * @param start_t The starting tangent of the function.
   * @param end     The ending value of the function.
   * @param end_t   The ending tangent of the function.
   */
  constexpr HermiteFnc(double start, double start_t, double end, double end_t);
  constexpr ~HermiteFnc() = default;

  /**
   * Calculate the y value of the hermite given x.
   *
   * @param  x The input value in the range of [0, 1].
   * @return The calculated y value.
   */
  constexpr double calc(double x) const override {
    double sum {0};
    for (size_t i = 0; i < coeffs.size(); ++i) {
      sum += coeffs.at(i) * std::pow(x, i);
    }
    return sum;
  }

  /**
   * Calculate the first derivative of the hermite given x using the power rule.
   *
   * @param  x The input value in the range of [0, 1].
   * @return The calculated first derivative.
   */
  constexpr double calc_d(double x) const override {
    double sum {0};
    for (size_t i = 1; i < coeffs.size(); ++i) {
      sum += coeffs.at(i) * i * std::pow(x, i - 1);
    }
    return sum;
  }

  /**
   * Calculate the second derivative of the hermite given x using the power rule.
   *
   * @param  x The input value in the range of [0, 1].
   * @return The calculated second derivative.
   */
  constexpr double calc_d2(double x) const override {
    double sum {0};
    for (size_t i = 2; i < coeffs.size(); ++i) {
      sum += coeffs.at(i) * i * (i - 1) * std::pow(x, i - 2);
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
template <>
constexpr HermiteFnc<3>::HermiteFnc(double start, double start_t, double end, double end_t) {
  double u = end - start;

  double a2 = 3.0 * u - 2.0 * start_t - end_t;
  double a3 = -2.0 * u + start_t + end_t;

  coeffs = {start, start_t, a2, a3};
}

template <>
constexpr HermiteFnc<5>::HermiteFnc(double start, double start_t, double end, double end_t) {
  double u = end - start - start_t;
  double v = end_t - start_t;

  double a3 = 10.0 * u - 4.0 * v;
  double a4 = -15.0 * u + 7.0 * v;
  double a5 = 6.0 * u - 3.0 * v;

  coeffs = {start, start_t, 0.0, a3, a4, a5};
}

/**
 * Specialization for a Parametric<HermiteFnc<N>> which allows construction using a start and end
 * state.
 */
template <size_t N> class Parametric<HermiteFnc<N>> : public Parametric<HermiteFnc<N>, true> {
public:
  /**
   * Helper method to construct a Piecewise<HermiteFnc<N> given a start and end state and their
   * respective stretch values. The tangents of the functions will be proportional to each other,
   * and the stretch controls their weight.
   *
   * @param start        The start state.
   * @param end          The end state.
   * @param startStretch The start stretch.
   * @param endStretch   The end stretch.
   */
  constexpr Parametric(const State& start, const State& end, double startStretch,
                       double endStretch) :
    Parametric<HermiteFnc<N>, true>(
      HermiteFnc<N>(start.x.convert(meter), cos(start.theta).convert(number) * startStretch,
                    end.x.convert(meter), cos(end.theta).convert(number) * endStretch),
      HermiteFnc<N>(start.y.convert(meter), sin(start.theta).convert(number) * startStretch,
                    end.y.convert(meter), sin(end.theta).convert(number) * endStretch)) {}

  /**
   * Helper method to construct a Parametric<HermiteFnc<N> given a start and end state and an
   * optional stretch value for both ends of the hermite. The tangents of the functions will be
   * proportional to each other, and the stretch controls their weight.
   *
   * @param start   The start state.
   * @param end     The end state.
   * @param stretch The stretch for both ends of the hermite (optional).
   */
  constexpr Parametric(const State& start, const State& end, double stretch = 1) :
    Parametric(start, end, stretch, stretch) {}

  constexpr ~Parametric() override = default;
};

/**
 * Helper function used to create a Piecewise<Hermite<N>> using an array of States. Strings a
 * hermite between each point with C1 continuity. The number of hermites in the piecewise is one
 * less than the number of points.
 *
 * @tparam P The type of Hermite, for example CubicHermite.
 * @tparam N The number of points provided.
 * @return a Piecewise<P, N-1>>.
 */
template <class P, size_t N>
requires std::same_as<P, Hermite<P::type::order>> constexpr auto make_piecewise(State(&&ip)[N]) {
  std::array<std::optional<P>, N - 1> p;
  for (size_t i = 0; i < N - 1; ++i) {
    p.at(i).emplace(ip[i], ip[i + 1]);
  }
  return Piecewise(std::move(p));
}
} // namespace lib7842