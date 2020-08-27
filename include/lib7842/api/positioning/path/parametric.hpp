#pragma once
#include "path.hpp"

namespace lib7842 {

/**
 * A parametric function is a one-dimensional function that maps y as a function of x. Examples are
 * a BezierFnc or HermiteFnc.
 */
class ParametricFnc {
public:
  /**
   * Calculate the y value of the function given x.
   *
   * @param  x The input value in the range of [0, 1].
   * @return The calculated y value.
   */
  virtual constexpr double calc(double x) const = 0;
  /**
   * Calculate the first derivative of the function given x using the power rule.
   *
   * @param  x The input value in the range of [0, 1].
   * @return The calculated first derivative.
   */
  virtual constexpr double calc_d(double x) const = 0;
  /**
   * Calculate the second derivative of the function given x using the power rule.
   *
   * @param  x The input value in the range of [0, 1].
   * @return The calculated second derivative.
   */
  virtual constexpr double calc_d2(double x) const = 0;
};

template <class T> concept IsParametricFnc = std::derived_from<ParametricFnc, T>;

/**
 * A Parametric is a two-dimensional path that uses two one-dimensional functions to map x and y as
 * a function of t.
 *
 * @tparam T The type of ParametricFnc.
 * @tparam B Used to facilitate specialization, setting the value to true will force the
 * specializations to be ignored, allowing for the specializations to inherit from this base
 * implementation.
 */
template <class T, bool B = false>
requires IsParametricFnc<T> class Parametric : public PathHelper<Parametric<T>> {
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

  using type = T;

protected:
  std::pair<T, T> p;
};

template <class T, bool D = false> Parametric(T&&, T&&) -> Parametric<T, D>;
}; // namespace lib7842
