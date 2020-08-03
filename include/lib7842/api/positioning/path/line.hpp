#pragma once
#include "lib7842/api/positioning/point/state.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "path.hpp"

namespace lib7842 {

class Line : public Path<State> {
public:
  Line(const Vector& istart, const Vector& iend) :
    start(istart, istart.angleTo(iend)), end(iend, start.theta) {}

  State calc(double t) const override {
    return start + (end - start) * t;
  }

  double curvature(double /*t*/) const override {
    return 0;
  }

  QLength length(double /*resolution*/) const override {
    return start.distTo(end);
  }

protected:
  const State start;
  const State end;
};

} // namespace lib7842