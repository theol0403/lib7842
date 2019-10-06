#pragma once
#include "main.h"
#include "point.hpp"

namespace lib7842 {

struct QState : public QPoint {
 public:
  QAngle theta {0_rad};

  using QPoint::QPoint;
  QState(const QLength& ix, const QLength& iy, const QAngle& itheta);
  QState(const QState& istate) = default;
  explicit QState(const QPoint& ipoint);
  virtual ~QState() override = default;

  virtual QState operator+(const QState& rhs) const;
  virtual QState operator-(const QState& rhs) const;
  virtual bool operator==(const QState& rhs) const;
  virtual bool operator!=(const QState& rhs) const;
};

} // namespace lib7842