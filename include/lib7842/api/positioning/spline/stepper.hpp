#pragma once
#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/deps/static_vector.h"

namespace lib7842 {
template <class T> concept ConstStepper = requires { T::N; };

template <class T, class U, class S> class Stepper {
public:
  constexpr Stepper(T&& ispline, S&& isampler) :
    spline(std::forward<T>(ispline)), sampler(std::forward<S>(isampler)) {}
  constexpr ~Stepper() = default;

  constexpr const std::remove_reference_t<T>& get() const { return spline; }

  template <class V = S> requires(!ConstStepper<V>) auto generate() const {
    return std::vector<State>(begin(), end());
  }

  template <class V = S> requires ConstStepper<V> consteval auto generate() const {
    std::static_vector<State, V::N> s;
    for (auto&& p : *this) {
      s.emplace_back(p);
    }
    return s;
  }

  template <class V = S> requires(!ConstStepper<V>) constexpr auto begin() const {
    return sampler.template begin<T>(spline);
  }

  template <class V = S> requires(!ConstStepper<V>) constexpr auto end() const {
    return sampler.template end<T>(spline);
  }

  template <class V = S> requires ConstStepper<V> consteval auto begin() const {
    return sampler.template begin<T>(spline);
  }

  template <class V = S> requires ConstStepper<V> consteval auto end() const {
    return sampler.template end<T>(spline);
  }

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
    constexpr ~iterator() = default;
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
  constexpr ~Count() = default;
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
    constexpr ~iterator() = default;
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
  constexpr ~Dist() = default;
  template <class P> constexpr auto begin(const P& ip) const { return iterator<P>(ip, d, 0.0); }
  template <class P> constexpr auto end(const P& ip) const { return iterator<P>(ip, d, 1.0); }
  const QLength d;
};

template <size_t C> class ConstCount {
  static_assert(C > 0, "StepBy::ConstCount: C must be greater than zero");
  template <class P>
  class iterator : public std::iterator<const std::forward_iterator_tag, State, size_t> {
  public:
    consteval iterator(const P& ip, size_t ii) : p(ip), i(ii) {}
    constexpr ~iterator() = default;
    consteval bool operator!=(const iterator& rhs) const { return i != (rhs.i + 1); }
    consteval State operator*() const { return p.calc(static_cast<double>(i) / C); }
    consteval State operator->() const { return *(*this); }
    consteval iterator& operator++() {
      ++i;
      return *this;
    }

  protected:
    const P& p;
    size_t i;
  };

public:
  consteval ConstCount() = default;
  constexpr ~ConstCount() = default;
  template <class P> consteval auto begin(const P& ip) const { return iterator<P>(ip, 0); }
  template <class P> consteval auto end(const P& ip) const { return iterator<P>(ip, C); }
  constexpr static size_t N = C + 1;
};

} // namespace StepBy
} // namespace lib7842
