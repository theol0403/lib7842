#pragma once
#include "lib7842/api/positioning/point/state.hpp"
#include "lib7842/api/positioning/point/vector.hpp"
#include "pathStepper.hpp"

namespace lib7842 {

class Path {
public:
  constexpr Path() = default;
  virtual ~Path() = default;

  constexpr virtual State calc(double t) const = 0;

  constexpr virtual double curvature(double t) const = 0;

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

  template <typename S> std::vector<State> generate(S&& s) const {
    std::vector<State> v;
    std::move(s.begin(*this), s.end(*this), std::back_inserter(v));
    return v;
  }
};
} // namespace lib7842