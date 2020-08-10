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
public:
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

  constexpr Count(int ic) : c(ic) {
    // ic < 0 ? true : throw std::invalid_argument("count must be greater than zero");
  }

  template <typename P> auto begin(const P& ip) const -> iterator<P> { return {ip, c, 0}; }
  template <typename P> auto end(const P& ip) const -> iterator<P> { return {ip, c, c}; }

protected:
  const int c;
};

constexpr Count T(double t) {
  return t > 0.0 && t <= 1.0
           ? static_cast<int>(1.0 / t)
           : throw std::invalid_argument("t must be greater than zero and less than or equal to 1");
}
} // namespace StepBy

// constexpr auto T(double t) { return [t](const auto& it) { return it.t + t;
// }; }

// constexpr auto Count(int c) {
//   return [c](const auto& it) {
//     return it.t + 1.0F / static_cast<double>(c);
//   };
// }

// template <typename T> constexpr auto Dist(T&& d) {
//   return [d2 = std::forward<T>(d)](const auto& it) {
//     return it.p.get().t_at_dist_travelled(it.t, d2);
//   };
// }
} // namespace lib7842
