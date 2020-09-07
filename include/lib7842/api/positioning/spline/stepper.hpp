#pragma once
#include "lib7842/api/positioning/point/state.hpp"

namespace lib7842 {

/**
 * A Stepper is a class that behaves like a container by providing begin and end methods. These
 * methods produce iterators which sample a spline according to some strategy. This gives the user a
 * way to expressively control how a spline is traversed and lazily defer spline calculation until
 * needed.
 *
 * A Stepper contains a spline and a sampler. The spline is contained by value or reference
 * depending on how it was passed to the Stepper constructor. A sampler is helper class which
 * describes how to step through the spline. There are a few samplers available in the `StepBy`
 * namespace: `Count` (how many steps from start to end), `T` (the increment in t from 0 to 1), and
 * `Dist` (the increment in distance).
 *
 * There are a few ways to use this class:
 * - If you want a Stepper to use as an iterator (for example, with a for-each loop or stl
 *   algorithm), you can either use the Stepper constructor or `Spline::step`.
 * - If you want to simply produce an array of points, you can use the generate method of this class
 *   or directly use `Spline::generate`.
 *
 * @tparam T The raw spline type.
 * @tparam U The spline storage type. Either `T` or `std::reference_wrapper<T>` depending on whether
 *           the spline was passed as an rvalue or lvalue, respectively. This parameter is
 *           automatically set according to the class template deduction guide.
 * @tparam S The type of sampler.
 */
template <class T, class U, class S> class Stepper {
public:
  /**
   * Create a new Stepper given a spline and a sampler.
   *
   * @param ispline  The spline to step over. Can be either an rvalue or lvalue.
   * @param isampler The sampler used to step through the spline.
   */
  constexpr Stepper(T&& ispline, S&& isampler) :
    spline(std::forward<T>(ispline)), sampler(std::forward<S>(isampler)) {}

  /**
   * Sample the entire spline according to the sampler, and return the resulting array of points.
   *
   * @return The array of points.
   */
  auto generate() const { return std::vector<State>(begin(), end()); }

  /**
   * Container iterator methods. These return an iterator to the beginning and end of the spline.
   *
   * @return An S::iterator<T> which directly samples the spline.
   */
  constexpr auto begin() const { return sampler.template begin<T>(spline); }
  constexpr auto end() const { return sampler.template end<T>(spline); }

protected:
  U spline;
  S sampler;
};

// class template deduction guide. If T is an rvalue, then the Stepper has an owning `T` member. If
// T is an lvalue, then the Stepper has a `std::reference_wrapper<T>` member.
template <class T, class S>
Stepper(T&&, S&&)
  -> Stepper<T,
             std::conditional_t<std::is_lvalue_reference_v<T>,
                                std::reference_wrapper<std::remove_reference_t<T>>, T>,
             S>;

/**
 * This namespace provides a collection of samplers to be given to `Stepper(spline, sampler)`,
 * `Spline::step(sampler)`, or `Spline::generate(sampler)`.
 */
namespace StepBy {

/**
 * A Count is a sampler which samples a certain number of steps across the spline. For example, if
 * the count is 100, then the increment used for `t` will be 0.01. However, since there is a
 * beginning and end to the spline, there will actually be 101 points sampled.
 */
class Count {
  /**
   * This class is the internal iterator which does the work to step through and sample the spline.
   */
  template <class T>
  class iterator : public std::iterator<const std::forward_iterator_tag, State, size_t> {
  public:
    constexpr iterator(const T& ip, size_t ic, size_t ii) : p(ip), c(ic), i(ii) {}
    constexpr bool operator!=(const iterator& rhs) const { return i != (rhs.i + 1); }
    constexpr State operator*() const { return p.calc(static_cast<double>(i) / c); }
    constexpr State operator->() const { return *(*this); }
    constexpr iterator& operator++() {
      ++i;
      return *this;
    }

  protected:
    const T& p;
    const size_t c;
    size_t i;
  };

public:
  /**
   * Create a new sampler that samples a certain number of points.
   *
   * @param ic How many points to sample across the spline. Since there is a beginning and end to
   *           the spline, there will actually be one additional point sampled. Must be positive and
   *           greater than zero.
   */
  consteval explicit Count(size_t ic) : c(ic) {
    ic > 0 ? true : throw std::invalid_argument("StepBy::Count: count must be greater than zero");
  }
  template <class T> constexpr auto begin(const T& ip) const { return iterator<T>(ip, c, 0); }
  template <class T> constexpr auto end(const T& ip) const { return iterator<T>(ip, c, c); }
  const size_t c;
};

/**
 * A T is a sampler which uses a constant increment for `t` to sample across the spline. It will
 * start at 0 and accumulate according to the increment until it reaches 1.0. Internally, it uses a
 * Count with the inverse of the increment. For example, if the increment is 0.01, then it will
 * return a Count(100), which in turn samples 101 points. If the increment can't properly fit into a
 * whole number, it will be rounded up, thus rounding down the Count.
 *
 * @param  it What increment to move `t` along the spline. Needs to be greater than zero and
 *            preferably have 1.0 as a multiple. Can't be larger than 1.0. Note that if it is 1.0,
 *            two points will be sampled - the start and the end.
 * @return A new sampler that samples along a path.
 */
consteval auto T(double it) {
  return Count(it > 0.0 && it <= 1.0
                 ? static_cast<size_t>(1.0 / it)
                 : throw std::invalid_argument(
                     "StepBy::T: t must be greater than zero and less than or equal to 1"));
}

/**
 * A Dist is a sampler which samples points along a spline that have have constant spacing.
 * Internally it keeps track of t, and for every step it uses the `t_at_dist_travelled` method to
 * calculate how much to increase `t`. The sampler starts with `t` at 0 and ends when `t` is greater
 * than 1.0.
 */
class Dist {
  template <class T>
  class iterator : public std::iterator<const std::forward_iterator_tag, State, double> {
  public:
    /**
     * This class is the internal iterator which does the work to step through and sample the
     * spline.
     */
    constexpr iterator(const T& ip, const QLength& id, double it) : p(ip), d(id), t(it) {}
    constexpr bool operator!=(const iterator& rhs) const { return static_cast<float>(t) <= rhs.t; }
    constexpr State operator*() const { return p.calc(t); }
    constexpr State operator->() const { return *(*this); }
    constexpr iterator& operator++() {
      t = p.t_at_dist_travelled(t, d);
      return *this;
    }

  protected:
    const T& p;
    const QLength d;
    double t;
  };

public:
  /**
   * Create a new sampler that samples points a certain distance apart.
   *
   * @param id What spacing to use between the points. Must be positive and greater than zero.
   */
  consteval explicit Dist(const QLength& id) : d(id) {
    id > 0_m ? true : throw std::invalid_argument("StepBy::Dist: dist must be greater than zero");
  }
  template <class T> constexpr auto begin(const T& ip) const { return iterator<T>(ip, d, 0.0); }
  template <class T> constexpr auto end(const T& ip) const { return iterator<T>(ip, d, 1.0); }
  const QLength d;
};

} // namespace StepBy
} // namespace lib7842
