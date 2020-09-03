#pragma once
#include "lib7842/api/positioning/point/state.hpp"

namespace lib7842 {

/**
 * A Stepper is a class that acts like an iterator which samples a spline. This provides a way to
 * expressively control how a spline is traversed.
 *
 * A Stepper contains a Spline and a StepBy. The spline is contained by value or reference depending
 * on how it was passed to the Stepper constructor. A StepBy is helper class which describes how to
 * step through the path.
 *
 * There are a few StepBy available in the `StepBy` namespace: `Count` (how many steps from start to
 * end), `T` (the increment in t from 0 to 1), and `Dist` (the increment in distance).
 *
 * There are a few ways to use this class:
 * - If you want a Stepper to use as an iterator (for example, with a for-each loop or std
 *   algorithm), you can either use the Stepper constructor or `Spline::step`.
 * - If you want to use a StepBy to produce an array of points, you can use the generate method of
 *   this class or directly use `Spline::generate`.
 *
 * @tparam T The raw spline type.
 * @tparam U The spline storage type. Either `T` or `std::reference_wrapper<T>` depending on whether
 *           the spline was passed as an rvalue or lvalue, respectively.
 * @tparam S The type of StepBy.
 */
template <class T, class U, class S> class Stepper {
public:
  /**
   * Create a new Stepper given a Spline and a StepBy.
   *
   * @param ispline  The spline to step over. Can be either an rvalue or lvalue.
   * @param isampler The StepBy used to step through the spline.
   */
  constexpr Stepper(T&& ispline, S&& isampler) :
    spline(std::forward<T>(ispline)), sampler(std::forward<S>(isampler)) {}

  /**
   * Sample the entire spline according to the StepBy, and return the resulting array of points.
   *
   * @return The array of points.
   */
  auto generate() const { return std::vector<State>(begin(), end()); }

  /**
   * Iterator methods, return an iterator to the beginning and end of the spline.
   *
   * @return An S::iterator which directly samples the spline.
   */
  constexpr auto begin() const { return sampler.template begin<T>(spline); }
  constexpr auto end() const { return sampler.template end<T>(spline); }

protected:
  U spline;
  S sampler;
};

template <class T, class S>
Stepper(T&&, S&&)
  -> Stepper<T,
             std::conditional_t<std::is_lvalue_reference_v<T>,
                                std::reference_wrapper<std::remove_reference_t<T>>, T>,
             S>;

namespace StepBy {

class Count {
  template <class P>
  class iterator : public std::iterator<const std::forward_iterator_tag, State, size_t> {
  public:
    constexpr iterator(const P& ip, size_t ic, size_t ii) : p(ip), c(ic), i(ii) {}
    constexpr bool operator!=(const iterator& rhs) const { return i != (rhs.i + 1); }
    constexpr State operator*() const { return p.calc(static_cast<double>(i) / c); }
    constexpr State operator->() const { return *(*this); }
    constexpr iterator& operator++() {
      ++i;
      return *this;
    }

  protected:
    const P& p;
    const size_t c;
    size_t i;
  };

public:
  consteval explicit Count(size_t ic) : c(ic) {
    ic > 0 ? true : throw std::invalid_argument("StepBy::Count: count must be greater than zero");
  }
  template <class P> constexpr auto begin(const P& ip) const { return iterator<P>(ip, c, 0); }
  template <class P> constexpr auto end(const P& ip) const { return iterator<P>(ip, c, c); }
  const size_t c;
};

consteval Count T(double t) {
  return Count(t > 0.0 && t <= 1.0
                 ? static_cast<size_t>(1.0 / t)
                 : throw std::invalid_argument(
                     "StepBy::T: t must be greater than zero and less than or equal to 1"));
}

class Dist {
  template <class P>
  class iterator : public std::iterator<const std::forward_iterator_tag, State, double> {
  public:
    constexpr iterator(const P& ip, const QLength& id, double it) : p(ip), d(id), t(it) {}
    constexpr bool operator!=(const iterator& rhs) const { return static_cast<float>(t) <= rhs.t; }
    constexpr State operator*() const { return p.calc(t); }
    constexpr State operator->() const { return *(*this); }
    constexpr iterator& operator++() {
      t = p.t_at_dist_travelled(t, d);
      return *this;
    }

  protected:
    const P& p;
    const QLength d;
    double t;
  };

public:
  consteval explicit Dist(const QLength& id) : d(id) {
    id > 0_m ? true : throw std::invalid_argument("StepBy::Dist: dist must be greater than zero");
  }
  template <class P> constexpr auto begin(const P& ip) const { return iterator<P>(ip, d, 0.0); }
  template <class P> constexpr auto end(const P& ip) const { return iterator<P>(ip, d, 1.0); }
  const QLength d;
};

} // namespace StepBy
} // namespace lib7842
