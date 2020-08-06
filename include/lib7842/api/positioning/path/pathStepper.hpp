#include "path.hpp"

namespace lib7842 {

template <typename T, typename U, typename S> class PathStepper {
protected:
  class iterator;

public:
  constexpr PathStepper(T&& ipath, S&& isampler) :
    path(std::forward<T>(ipath)), sampler(std::forward<S>(isampler)) {}

  constexpr iterator begin() { return {*this, 0.0}; }
  constexpr iterator end() { return {*this, 1.0}; }

  std::vector<State> generate() const {
    std::vector<State> s;
    std::move(begin(), end(), std::back_inserter(s));
    return s;
  }

protected:
  const U path;
  const S& sampler;

  friend class iterator;
  class iterator : public std::iterator<const std::forward_iterator_tag, State, double> {
  public:
    constexpr iterator(const PathStepper& ip, double it) : p(ip), t(it) {}

    constexpr bool operator!=(const iterator& rhs) { return static_cast<float>(t) <= rhs.t; }
    State operator*() { return static_cast<T>(p.path).calc(t); }
    State operator->() { return *(*this); }

    constexpr iterator& operator++() {
      t = p.sampler(*this);
      return *this;
    }

    const PathStepper& p;
    double t;
  };
};

template <typename T, typename S>
PathStepper(T&&, S &&)
  -> PathStepper<T,
                 std::conditional_t<std::is_lvalue_reference_v<T>,
                                    std::reference_wrapper<std::remove_reference_t<T>>, T>,
                 S>;

namespace StepBy {
constexpr auto T(double t) {
  return [t](const auto& it) {
    return it.t + t;
  };
}

constexpr auto Count(int c) {
  return [c](const auto& it) {
    return it.t + 1.0F / static_cast<double>(c);
  };
}
} // namespace StepBy

} // namespace lib7842