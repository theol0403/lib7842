#pragma once
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include "pathStepper.hpp"

namespace lib7842 {

class Path {
public:
  constexpr Path() = default;
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

  template <typename S> requires(!ConstStepper<S>) std::vector<State> generate(S&& s) const {
    return PathStepper(*this, std::forward<S>(s)).generate();
  }

  template <typename S>
  requires ConstStepper<S> consteval std::array<State, S::N> generate(S&& s) const {
    return PathStepper(*this, std::forward<S>(s)).generate();
  }
};

template <typename CRTP> class PathHelper : public Path {
public:
  constexpr PathHelper() = default;
  template <typename S> constexpr auto step(S&& s) const& {
    return PathStepper(static_cast<const CRTP&>(*this), s);
  }
  template <typename S> constexpr auto step(S&& s) && {
    return PathStepper(static_cast<CRTP&&>(*this), s);
  }
};
} // namespace lib7842