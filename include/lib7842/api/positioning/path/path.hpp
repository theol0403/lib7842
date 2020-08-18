#pragma once
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include "stepper.hpp"

namespace lib7842 {

class Path {
public:
  constexpr virtual ~Path() = default;

  constexpr virtual State calc(double t) const = 0;

  constexpr virtual QCurvature curvature(double t) const = 0;

  constexpr virtual QLength velocity(double /*t*/) const { return length(); }

  constexpr virtual QLength length(double resolution = 100) const {
    QLength len {0.0};
    for (size_t i = 0; i < resolution; i++) {
      len += calc(i / resolution).distTo(calc((i + 1) / resolution));
    }
    return len;
  }

  constexpr virtual double t_at_dist_travelled(double t, const QLength& dist) const {
    return t + (dist / velocity(t).abs()).convert(number);
  }
};

template <typename CRTP> struct RuntimePath {
  template <typename S> requires(!ConstStepper<S>) constexpr auto step(S&& s) const& {
    return Stepper(static_cast<const CRTP&>(*this), std::forward<S>(s));
  }
  template <typename S> requires(!ConstStepper<S>) constexpr auto step(S&& s) && {
    return Stepper(static_cast<CRTP&&>(*this), std::forward<S>(s));
  }
  template <typename S> requires(!ConstStepper<S>) std::vector<State> generate(S&& s) const& {
    return Stepper(static_cast<const CRTP&>(*this), std::forward<S>(s)).generate();
  }
  template <typename S> requires(!ConstStepper<S>) std::vector<State> generate(S&& s) && {
    return Stepper(static_cast<CRTP&&>(*this), std::forward<S>(s)).generate();
  }
};
template <typename CRTP> struct ConstPath {
  consteval ConstPath() = default;
  template <typename S> requires ConstStepper<S> consteval auto step(S&& s) const& {
    return Stepper(static_cast<const CRTP&>(*this), std::forward<S>(s));
  }
  template <typename S> requires ConstStepper<S> consteval auto step(S&& s) && {
    return Stepper(static_cast<CRTP&&>(*this), std::forward<S>(s));
  }
  template <typename S>
  requires ConstStepper<S> consteval std::array<State, S::N> generate(S&& s) const& {
    return Stepper(static_cast<const CRTP&>(*this), std::forward<S>(s)).generate();
  }
  template <typename S>
  requires ConstStepper<S> consteval std::array<State, S::N> generate(S&& s) && {
    return Stepper(static_cast<CRTP&&>(*this), std::forward<S>(s)).generate();
  }
};
template <typename CRTP>
struct PathHelper : public RuntimePath<CRTP>, public ConstPath<CRTP>, public Path {
  constexpr ~PathHelper() override = default;
  using RuntimePath<CRTP>::step;
  using ConstPath<CRTP>::step;
  using RuntimePath<CRTP>::generate;
  using ConstPath<CRTP>::generate;
};
} // namespace lib7842