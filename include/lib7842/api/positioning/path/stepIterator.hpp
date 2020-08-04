#include <functional>
#include <iterator>
#include <type_traits>

namespace lib7842 {
template <typename T = std::nullptr_t> class StepIterator {
public:
  template <typename U> static constexpr auto create(U&& ipath) {
    return StepIterator<std::conditional_t<std::is_lvalue_reference_v<U>,
                                           std::reference_wrapper<std::remove_reference_t<U>>, U>>(
      std::forward<U>(ipath));
  }

protected:
  StepIterator(T&& ipath) : path(std::forward<T>(ipath)) {}
  const T path;
};
} // namespace lib7842