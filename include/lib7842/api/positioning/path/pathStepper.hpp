#pragma once
#include "lib7842/api/positioning/point/state.hpp"

namespace lib7842 {

template <typename T, typename U, typename S> class PathStepper {
public:
  constexpr PathStepper(T&& ipath, S&& isampler) :
    path(std::forward<T>(ipath)), sampler(std::forward<S>(isampler)) {}

  constexpr auto begin() const { return sampler.begin(static_cast<const T&>(path)); }
  constexpr auto end() const { return sampler.end(static_cast<const T&>(path)); }

  constexpr const std::remove_reference_t<T>& get() const { return path; }

  std::vector<State> generate() const {
    std::vector<State> s;
    std::move(begin(), end(), std::back_inserter(s));
    return s;
  }

protected:
  const U path;
  const S sampler;
};

template <typename T, typename S>
PathStepper(T&&, S &&)
  -> PathStepper<T,
                 std::conditional_t<std::is_lvalue_reference_v<T>,
                                    std::reference_wrapper<std::remove_reference_t<T>>, T>,
                 S>;

namespace StepBy {

class Count {
  template <typename P> class iterator;

public:
  consteval Count(int ic) : c(ic) {
    ic > 0 ? true : throw std::invalid_argument("StepBy::Count: count must be greater than zero");
  }

  template <typename P> auto begin(const P& ip) const -> iterator<P> { return {ip, c, 0}; }
  template <typename P> auto end(const P& ip) const -> iterator<P> { return {ip, c, c}; }

protected:
  const int c;

private:
  template <typename P>
  class iterator : public std::iterator<const std::forward_iterator_tag, State, int> {
  public:
    constexpr iterator(const P& ip, int ic, int ii) : p(ip), c(ic), i(ii) {}

    constexpr bool operator!=(const iterator& rhs) const { return i != (rhs.i + 1); }
    constexpr State operator*() const { return p.calc(static_cast<double>(i) / c); }
    constexpr State operator->() const { return *(*this); }

    constexpr iterator& operator++() {
      ++i;
      return *this;
    }

  protected:
    const P& p;
    const int c;
    int i;
  };
};

consteval Count T(double t) {
  return t > 0.0 && t <= 1.0
           ? static_cast<int>(1.0 / t)
           : throw std::invalid_argument(
               "StepBy::T: t must be greater than zero and less than or equal to 1");
}

class Dist {
  template <typename P> class iterator;

public:
  consteval Dist(const QLength& id) : d(id) {
    id > 0_m ? true : throw std::invalid_argument("StepBy::Dist: dist must be greater than zero");
  }

  template <typename P> auto begin(const P& ip) const -> iterator<P> { return {ip, d, 0.0}; }
  template <typename P> auto end(const P& ip) const -> iterator<P> { return {ip, d, 1.0}; }

protected:
  const QLength d;

private:
  template <typename P>
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
};

} // namespace StepBy
} // namespace lib7842
