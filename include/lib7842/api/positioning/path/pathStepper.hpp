#include "path.hpp"

namespace lib7842 {

template <typename T, typename U, typename S> class PathStepper {

public:
  constexpr PathStepper(T&& ipath, S&& isampler) :
    path(std::forward<T>(ipath)), sampler(std::forward<S>(isampler)) {}

  constexpr auto begin() const { return sampler.begin(static_cast<T>(path)); }
  constexpr auto end() const { return sampler.end(static_cast<T>(path)); }
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

    constexpr bool operator!=(const iterator& rhs) { return i != (rhs.i + 1); }
    State operator*() { return p.calc(i / c); }
    State operator->() { return *(*this); }

    constexpr iterator& operator++() {
      ++i;
      return *this;
    }

  protected:
    const P& p;
    const int c;
    int i;
  };

  constexpr Count(int ic) : c(ic) {}
  template <typename P> auto begin(const P& ip) const -> iterator<P> { return {ip, c, 0}; }
  template <typename P> auto end(const P& ip) const -> iterator<P> { return {ip, c, c}; }

protected:
  const int c;
};
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
