#include "lib7842/api/positioning/point/vector.hpp"
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>

namespace lib7842 {

template <typename Prod = Vector, typename T = std::nullptr_t, typename Sampler = nullptr_t>
class StepIterator {
public:
  template <typename U, typename S> static constexpr auto create(U&& ipath, S&& isampler) {
    return StepIterator<typename std::remove_reference_t<U>::prod,
                        std::conditional_t<std::is_lvalue_reference_v<U>,
                                           std::reference_wrapper<std::remove_reference_t<U>>, U>,
                        S>(std::forward<U>(ipath), std::forward<S>(isampler));
  }

  class iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Prod;
    using difference_type = double;
    using pointer = Prod*;
    using reference = Prod&;

    bool operator!=(const iterator& rhs) {
      return t <= rhs.t;
    }

    Prod operator*() {
      return container->calc(t);
    }
    Prod operator->() {
      return container->calc(t);
    }

    iterator& operator++() {
      t += container->sampler(t, container->path);
      return *this;
    }
    iterator operator++(int) {
      return ++(*this);
    }

    iterator& operator--() {
      t -= container->sampler(t, container->path);
      return *this;
    }
    iterator operator--(int) {
      return ++(*this);
    }

    iterator(StepIterator<Sampler, Prod, T>* icontainer, double it = 0.0) :
      container(icontainer), t(it) {}

    StepIterator<Sampler, Prod, T>* container {nullptr};
    double t;
  };

  iterator begin() {
    return {this, 0.0};
  }

  iterator end() {
    return {this, 1.0};
  }

  // protected:
  StepIterator(T&& ipath, Sampler&& isampler) :
    path(std::forward<T>(ipath)), sampler(std::forward<Sampler>(isampler)) {}
  const T path;
  const Sampler& sampler;
};
} // namespace lib7842