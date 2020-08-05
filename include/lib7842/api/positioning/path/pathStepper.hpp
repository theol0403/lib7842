#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>

namespace lib7842 {

template <typename T, typename U, typename S> class PathStepper {
protected:
  class iterator;

public:
  PathStepper(T&& ipath, S&& isampler) :
    path(std::forward<T>(ipath)), sampler(std::forward<S>(isampler)) {}

  iterator begin() { return {*this, 0.0}; }
  iterator end() { return {*this, 1.0}; }

protected:
  const U path;
  const S& sampler;

  friend class iterator;
  class iterator : std::iterator<std::forward_iterator_tag, State, double> {
  public:
    iterator(const PathStepper& ip, double it) : p(ip), t(it) {}

    bool operator!=(const iterator& rhs) { return t <= rhs.t; }

    State operator*() { return static_cast<T>(p.path).calc(t); }
    State operator->() { return *(*this); }

    iterator& operator++() {
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
} // namespace lib7842