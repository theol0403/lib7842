#pragma once
#include "lib7842/api/positioning/point/state.hpp"
#include <array>
#include <vector>

namespace lib7842 {

template <bool Const, typename T> class PathContainer {
public:
  template <size_t N> consteval PathContainer(const std::array<State, N>& ip) : p(ip) {}
  PathContainer(const std::vector<State>& ip) : p(ip) {}

  operator const std::vector<State>&() const {
    if constexpr (Const) {
      return {p.start(), p.end()};
    } else {
      return p;
    }
  }

  template <bool U = Const, typename = std::enable_if_t<U>> consteval operator const T&() const {
    return p;
  }

protected:
  T p;
};

template <size_t N>
PathContainer(const std::array<State, N>&) -> PathContainer<true, std::array<State, N>>;
PathContainer(const std::vector<State>&)->PathContainer<false, std::vector<State>>;
} // namespace lib7842