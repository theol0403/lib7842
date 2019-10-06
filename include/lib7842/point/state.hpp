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

  virtual QState operator+(const QState& rhs) const;
  virtual QState operator-(const QState& rhs) const;
  virtual bool operator==(const QState& rhs) const;
  virtual bool operator!=(const QState& rhs) const;
};

} // namespace lib7842