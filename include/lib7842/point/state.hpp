#pragma once
#include "main.h"
#include "point.hpp"

namespace lib7842 {

class QState : public Vector {
 public:
  QAngle theta {0_rad};

  using Vector::Vector;
  QState(const QState& istate) = default;
  virtual ~QState() = default;

  QState(const QLength& ix, const QLength& iy, const QAngle& itheta);
  explicit QState(const Vector& ipoint);

  QState operator+(const QState& rhs) const;
  QState operator-(const QState& rhs) const;
  bool operator==(const QState& rhs) const;
  bool operator!=(const QState& rhs) const;
};

} // namespace lib7842