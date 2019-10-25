#pragma once
#include "main.h"
#include "point.hpp"

namespace lib7842 {

class State : public Vector {
public:
  QAngle theta {0_rad};

  using Vector::Vector;
  State(const State& istate) = default;
  virtual ~State() = default;

  State(const QLength& ix, const QLength& iy, const QAngle& itheta);
  explicit State(const Vector& ipoint);

  State operator+(const State& rhs) const;
  State operator-(const State& rhs) const;
  bool operator==(const State& rhs) const;
  bool operator!=(const State& rhs) const;
};

} // namespace lib7842