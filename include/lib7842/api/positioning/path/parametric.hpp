#pragma once
#include "bezier.hpp"
#include "hermite.hpp"
#include "path.hpp"

namespace lib7842 {

/**
 * A parametric function is a one-dimensional function that maps y as a function of x. Examples of
 * this is a Bezier or Hermite.
 */
template <typename T> concept ParametricFunction = requires(T t) {
  { t.calc(0.0) }
  ->std::convertible_to<double>;
};

/**
 * A Parametric is a two-dimensional path that uses two one-dimensional functions to map x and y as
 * a function of t.
 *
 * @tparam T The type of ParametricFunction.
 */
template <ParametricFunction T> class Parametric : public PathHelper<Parametric<T>> {
public:
  /**
   * Create a Parametric given two one-dimensional functions.
   *
   * @param x The function that maps x as a function of t.
   * @param y The function that maps y as a function of t.
   */
  constexpr Parametric(T&& x, T&& y) : p(std::forward<T>(x), std::forward<T>(y)) {}
  constexpr ~Parametric() override = default;

  /**
   * Sample the point along the path given t. The angle of the point is equal to the arctan of the
   * derivatives of the two functions.
   *
   * @param  t Where along the path to sample, in the range of [0, 1].
   * @return The sampled point at t.
   */
  constexpr State calc(double t) const override {
    QLength x_t = p.first.calc(t) * meter;
    QLength y_t = p.second.calc(t) * meter;

    QLength x1_t = p.first.calc_d(t) * meter;
    QLength y1_t = p.second.calc_d(t) * meter;

    return State(x_t, y_t, atan2(y1_t, x1_t));
  }

  /**
   * Sample the curvature of the path at t. Curvature is the inverse of the radius.
   * https://en.wikipedia.org/wiki/Curvature#In_terms_of_a_general_parametrization
   *
   * @param  t Where along the path to sample, in the range of [0, 1]
   * @return the curvature at t
   */
  constexpr QCurvature curvature(double t) const override {
    QLength x_d = p.first.calc_d(t) * meter;
    QLength y_d = p.second.calc_d(t) * meter;
    QLength x_d_2 = p.first.calc_d2(t) * meter;
    QLength y_d_2 = p.second.calc_d2(t) * meter;
    return ((x_d * y_d_2 - y_d * x_d_2) / pow<3>(sqrt(x_d * x_d + y_d * y_d)));
  }

  /**
   * Sample the velocity of the path at t. Velocity is the ratio between distance traveled and
   * change in t. In this case, the velocity is found by taking the hypotenuse of the derivative of
   * the two functions.
   *
   * @param t Where along the path to sample, in the range of [0, 1]
   * @return the velocity at t
   */
  constexpr QLength velocity(double t) const override {
    return sqrt(square(p.first.calc_d(t) * meter) + square(p.second.calc_d(t) * meter));
  }

  /**
   * Helper method to construct a Piecewise<Hermite<N> given a start and end state and their
   * respective stretch values. The tangents of the functions will be proportional to each other,
   * and the stretch controls their weight. This method is enabled when T is a Hermite.
   *
   * @param  start        The start state.
   * @param  end          The end state.
   * @param  startStretch The start stretch.
   * @param  endStretch   The end stretch.
   */
  template <typename U = T, typename = std::enable_if_t<std::is_base_of_v<Hermite<U::order>, U>>>
  constexpr Parametric(const State& start, const State& end, double startStretch,
                       double endStretch) :
    p(U(start.x.convert(meter), cos(start.theta).convert(number) * startStretch,
        end.x.convert(meter), cos(end.theta).convert(number) * endStretch),
      U(start.y.convert(meter), sin(start.theta).convert(number) * startStretch,
        end.y.convert(meter), sin(end.theta).convert(number) * endStretch)) {}

  /**
   * Helper method to construct a Piecewise<Hermite<N> given a start and end state and an optional
   * stretch value for both ends of the hermite. The tangents of the functions will be proportional
   * to each other, and the stretch controls their weight. This method is enabled when T is a
   * Hermite.
   *
   * @param  start     The start state.
   * @param  end       The end state.
   * @param  stretch   The stretch for both ends of the hermite (optional).
   */
  template <typename U = T, typename = std::enable_if_t<std::is_base_of_v<Hermite<U::order>, U>>>
  constexpr Parametric(const State& start, const State& end, double stretch = 1) :
    Parametric(start, end, stretch, stretch) {}

  /**
   * Helper method to construct a Piecewise<Bezier<N>> given an array of control points. Separates
   * the x and y component of the control points and delegates them to their respective functions.
   *
   * @param  ctrls     The array of control points.
   */
  template <typename U = T, size_t N = U::order,
            typename = std::enable_if_t<std::is_same_v<Bezier<N>, U>>>
  constexpr explicit Parametric(const Vector (&ctrls)[N + 1]) :
    p(Bezier(process(ctrls, [](const auto& ip) { return ip.x.convert(meter); })),
      Bezier(process(ctrls, [](const auto& ip) { return ip.y.convert(meter); }))) {}

protected:
  std::pair<T, T> p;

private:
  template <size_t N> constexpr auto process(const Vector (&ctrls)[N], auto&& f) {
    std::array<double, N> t;
    std::transform(std::begin(ctrls), std::end(ctrls), std::begin(t), f);
    return t;
  }
};

template <ParametricFunction T> Parametric(T&&, T &&) -> Parametric<T>;
template <size_t N> Parametric(const Vector (&)[N]) -> Parametric<Bezier<N - 1>>;

}; // namespace lib7842
