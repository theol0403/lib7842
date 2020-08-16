#pragma once
#include "lib7842/api/other/units.hpp"
#include "lib7842/api/positioning/point/state.hpp"
#include "okapi/api/units/QSpeed.hpp"

namespace lib7842 {
struct Waypoint : public State {
  QCurvature curvature {0 / meter};
  QSpeed velocity {0_mps};

  using State::State;
  constexpr Waypoint(const QLength& ix, const QLength& iy) : State(ix, iy, 0_deg) {}
  constexpr explicit Waypoint(const State& ipoint) : State(ipoint) {};
};
} // namespace lib7842