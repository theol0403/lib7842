#include "lib7842/api/positioning/point/vector.hpp"
#include <functional>
#include <iterator>
#include <type_traits>

namespace lib7842 {

template <typename Sampler, typename Prod = Vector, typename T = std::nullptr_t>
class StepIterator {
public:
  class iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = double;
    using difference_type = double;
    using pointer = double*;
    using reference = double&;

    bool operator!=(const iterator& rhs) {
      return t <= rhs.t;
    }

    double operator*() {
      return t;
    }
    double operator->() {
      return t;
    }

    iterator& operator++() {
      t += Sampler::step(t, container);
      return *this;
    }
    iterator operator++(int) {
      return ++(*this);
    }

    iterator& operator--() {
      t -= Sampler::step(t, container);
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

  template <typename S, typename U> static constexpr auto create(U&& ipath) {
    return StepIterator<S, typename U::prod,
                        std::conditional_t<std::is_lvalue_reference_v<U>,
                                           std::reference_wrapper<std::remove_reference_t<U>>, U>>(
      std::forward<U>(ipath));
  }

protected:
  StepIterator(T&& ipath) : path(std::forward<T>(ipath)) {}
  const T path;
};
} // namespace lib7842