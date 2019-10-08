#pragma once
#include "main.h"
#include "point.hpp"

namespace lib7842 {

class QState : public QPoint {
 public:
  QAngle theta {0_rad};

  using QPoint::QPoint;
  QState(const QState& istate) = default;
  virtual ~QState() = default;

  QState(const QLength& ix, const QLength& iy, const QAngle& itheta);
  explicit QState(const QPoint& ipoint);

  QState operator+(const QState& rhs) const;
  QState operator-(const QState& rhs) const;
  bool operator==(const QState& rhs) const;
  bool operator!=(const QState& rhs) const;
};

} // namespace lib7842