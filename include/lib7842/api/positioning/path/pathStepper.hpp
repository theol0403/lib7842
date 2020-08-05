#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>

namespace lib7842 {

template <typename T, typename S> class PathStepper {
public:
  class iterator : std::iterator<std::bidirectional_iterator_tag, State, double> {
  public:
    iterator(PathStepper<S, T>* icontainer, double it = 0.0) : container(icontainer), t(it) {}

    bool operator!=(const iterator& rhs) { return t <= rhs.t; }

    State operator*() { return container->calc(t); }
    State operator->() { return container->calc(t); }

    iterator& operator++() {
      t += container->sampler(t, container->path);
      return *this;
    }

    iterator& operator--() {
      t -= container->sampler(t, container->path);
      return *this;
    }

    iterator operator++(int) { return ++(*this); }
    iterator operator--(int) { return --(*this); }

  protected:
    PathStepper<S, T>* container {nullptr};
    double t;
  };

  PathStepper(T&& ipath, S&& isampler) :
    path(std::forward<T>(ipath)), sampler(std::forward<S>(isampler)) {}

  iterator begin() { return {this, 0.0}; }

  iterator end() { return {this, 1.0}; }

  // protected:
  const T path;
  const S& sampler;
};

template <typename T, typename S>
PathStepper(T&&, S &&)
  -> PathStepper<std::conditional_t<std::is_lvalue_reference_v<T>,
                                    std::reference_wrapper<std::remove_reference_t<T>>, T>,
                 S>;
} // namespace lib7842