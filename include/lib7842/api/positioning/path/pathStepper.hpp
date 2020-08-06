#include "path.hpp"

namespace lib7842 {

template <typename T, typename U, typename S> class PathStepper {

public:
  constexpr PathStepper(T&& ipath, S&& isampler) :
    path(std::forward<T>(ipath)), sampler(std::forward<S>(isampler)) {}

  constexpr typename S::template iterator<T> begin() const { return sampler.begin(path); }
  constexpr typename S::template iterator<T> end() const { return sampler.end(path); }
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

template <typename P>
class iteratorBase : public std::iterator<const std::forward_iterator_tag, State, double> {
public:
  constexpr iteratorBase(const P& ip) : p(ip) {}

  const P& p;
};

class Count {
public:
  constexpr Count(int ic) : c(ic) {}

  template <typename P> class iterator : public iteratorBase<P> {
  public:
    constexpr iterator(const P& ip, int ic, int ii) : iteratorBase<P>(ip), c(ic), i(ii) {}

    constexpr bool operator!=(const iterator& rhs) { return i != (rhs.i + 1); }
    State operator*() { return static_cast<P>(iteratorBase<P>::p).calc(i / c); }
    State operator->() { return *(*this); }

    constexpr iterator& operator++() {
      i++;
      return *this;
    }

    const int c;
    int i;
  };

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
